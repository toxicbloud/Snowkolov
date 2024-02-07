#iChannel0 "file://cubemaps/{}.jpg"
#iChannel0::Type "CubeMap"

// Antonin Rousseau

// Paramètres généraux du dessin
const float PI = 3.14159;      // Valeur de PI
const int nb_spheres = 24;            // Nombre de sphères
vec4 a_spheres[nb_spheres];     // Tableau des sphères
float attraction = 1.0;          // Coefficient d'attraction entre les sphères
bool coloriageDistance = false; // Coloriage selon la distance à la caméra
bool coloriageIterations = false;
bool coloriageNormales = false;
bool coloriageReflexion = false;
bool coloriageRefraction = false;
bool coloriageIlumination = true;
float reflexion = 0.5;
float refraction = 1.1;
float transparence = 0.5;

float rayonDeFusion = 1.0;

float smin(float dA, float dB, float r) {
    float c = clamp(0.5 + 0.5 * (dB - dA) / r, 0.0, 1.0);
    return mix(dB, dA, c) - r * c * (1.0 - c);
}

// couleur lumiere ambiante
vec4 ambiant = vec4(0.1, 0.1, 0.1, 0.2);

// lumiere ponctuelle
vec3 lumiere = vec3(10.0,10.0,5.0);
vec4 lumiereColor = vec4(1.0f, 0.95f, 0.0f, 1.0f);


float UnionSI(float d1, float d2) {
    return min(d1, d2);
}
float IntersectionSI(float d1, float d2) {
    return max(d1, d2);
}

float SoustractionSI(float d1, float d2) {
    return max(d1, -d2);
}

// Fonction de distance de p à une sphère définie par son centre et son rayon
float dstSphere(vec3 p, vec3 centre, float rayon) {
    // ... Remplacer par la distance signée à la sphère
    vec3 d = p - centre;
    return length(d) - rayon;
}

// Fonction de rotation d'un point p autour de l'axe Y selon un angle donné
vec3 rotationY(vec3 p, float angle) {
    float ca = cos(angle);
    float sa = sin(angle);
    return vec3(ca * p.x - sa * p.z, p.y, sa * p.x + ca * p.z);
}

// Fonction de distance de p à une boîte définie par son centre et sa 1/2 diagonale
float dstBoite(vec3 p, vec3 centre, vec3 diag, float angleY) {
    // ... Remplacer par la distance signée à la boîte
    vec3 d = abs(rotationY(p, angleY) - centre); // on remet le point dans le repère de la boîte
    // on regarde avec la diagonale
    d = d - diag;
    // on prend la plus grande distance
    if(length(d) > 0.0)
        return max(max(d.x, d.y), d.z);
        // return length(max(d, vec3(0.0)));
    return length(d);
}

float dstCadre(vec3 p, vec3 centre, vec3 diag, float angleY, float epaisseur) {
    float dst = dstBoite(p, centre, diag, angleY);
    float dst2 = dstBoite(p, centre, diag + vec3(1.0, -epaisseur, -epaisseur), angleY);
    float cadre = SoustractionSI(dst, dst2);
    float cadre2 = SoustractionSI(cadre, dstBoite(p, centre, diag + vec3(-epaisseur, 1.0, -epaisseur), angleY));
    float cadre3 = SoustractionSI(cadre2, dstBoite(p, centre, diag + vec3(-epaisseur, -epaisseur, 1.0), angleY));
    return cadre3;
    // return SoustractionSI(dst2,dst);
}

// Fonction de distance de p à un ensemble de sphères
float dstSpheres(vec3 p) {
    // Distance initiale
    float res = dstSphere(p, a_spheres[0].xyz, a_spheres[0].w); // ... Ajouter le calcul de distance à la première sphère
    // Parcours des autres sphères
    for(int i = 1; i < nb_spheres; ++i) {
        float dst = dstSphere(p, a_spheres[i].xyz, a_spheres[i].w); // ... Ajouter le calcul de distance à la ième sphère
        res = smin(res, dst,rayonDeFusion);
    }
    return res;
}

// Construction de la matrice caméra définie par
// son centre optique  co
// son point cible     ci 
// son angle de roulis ar
mat3 construitCamera(in vec3 co, in vec3 ci, float ar) {
    vec3 cw;       // ... Remplacer par le calcul de la direction du regard : axe Z caméra
    vec3 cp;       // ... Remplacer par le calcul de la rotation de la verticale du monde par le roulis caméra    
    vec3 cu;       // ... Remplacer par le calcul de l'axe X caméra
    vec3 cv;       // ... Remplacer par le calcul de l'axe Y caméra
    // calcul de la direction du regard
    cw = normalize(ci - co);
    // calcul de la rotation de la verticale du monde par le roulis caméra ar    
    // cp = vec3(cos(ar), sin(ar), 0.0);
    cp = vec3(0.0, 1.0, 0.0);
    // calcul de l'axe X caméra
    cu = normalize(cross(cw, cp));
    // calcul de l'axe Y caméra
    cv = cross(cu, cw);
    return mat3(cu, cv, cw);
}

// Fonction principale d'affichage du pixel à la position fragCoord
// -> Retourne la couleur finale dans fragColor
void mainImage(out vec4 fragColor, in vec2 fragCoord) {
    // Couleur initiale
    fragColor = vec4(0.0); 

    // Récupération de la position souris dans [-1:1]²
    vec2 souris = 2.0 * iMouse.xy / iResolution.xy - 1.0;

    // Définition de la caméra à partir de son point cible
    vec3 cibleCamera = vec3(0.0); // Point visé par la caméra (centre monde par défaut)
    float focale = 2.0;            // Distance focale
    float angle = (iTime / 2.0);   // Angle de positionnement de la caméra autour du point cible (rotation sur verticale)
    float distanceCamera = 10.0;   // Distance de la caméra au point cible
    float angleRoulis = 0.0;       // Angle de roulis autour de l'axe optique
    // Position du centre optique de la caméra avec prise en compte de la souris
    vec3 centreCamera = cibleCamera + vec3(distanceCamera * cos(angle + PI * souris.x),  // ... Compléter la rotation avec la position x de la souris
    10.0 * souris.y, distanceCamera * sin(angle + PI * souris.x)); // ... Compléter la rotation avec la position x de la souris
    float distanceCameraCible = distance(centreCamera, cibleCamera);

    // Construction de la matrice Caméra vers Monde
    mat3 camera = construitCamera(centreCamera, cibleCamera, angleRoulis); 

    // Position du pixel replacé dans le repère unitaire [-1:1] centré au milieu de l'image
    vec2 p = (2.0 * fragCoord - iResolution.xy) / iResolution.y; // La division permet de conserver les bonnes proportions en x et y

    // Définition d'une boite par son centre et sa demi diagonale
    vec3 centreB = vec3(0.0, sin(iTime) + 2.0, 0.0);

    vec3 diagB = vec3(0.75);
    vec3 coulB = vec3(0.0, 0.0, 1.0); // Couleur unie de la boîte
    float angleB = 5.0 * iTime;         // Angle de rotation de la boîte autour de la verticale

    // Creation de sphères alignées en x
    float nbs = float(nb_spheres);
    for(int i = 0; i < nb_spheres; ++i) {
        float ii = float(i);
        a_spheres[i] = vec4(6.0 * (2.0 * (ii + 0.5) / nbs - 1.0), 2.0 * sin(ii + iTime), 2.0 * cos(iTime), 0.75);
    }
    // Définition de la couleur des sphères
    vec3 coulS = vec3(1.0, 0.0, 0.0);

    // Construction du rayon issu du pixel courant (position p dans le plan caméra et focale en profondeur)
    vec3 rd = camera * normalize(vec3(p, focale));

    // 
    // Calcul de la plus courte distance aux objets le long du rayon
    // 

    // Précision de détection des contacts du rayon avec les objets
    float prec = 1e-3;

    // Évaluation du point de départ
    vec3 pt = centreCamera; // On part du centre optique
    float t = focale;       // On se place sur le plan image
    // Distance minimale du point courant aux sphères et à la boîte

    float epaisseurCadre = 0.2;
    float dstB = dstCadre(pt, centreB, diagB, angleB, epaisseurCadre);
    float dstS = dstSpheres(pt);
    float dst = smin(dstB, dstS,rayonDeFusion);
    int nbIters = 0;
    vec3 normalObjet = vec3(0.0);

    float constanteMagique = 10e-4 * sqrt(3.0) / 3.0;

    vec4 v1 = constanteMagique * vec4(-1, -1, 1, 0.0);
    vec4 v2 = constanteMagique * vec4(-1, 1, -1, 0.0);
    vec4 v3 = constanteMagique * vec4(1, -1, -1, 0.0);
    vec4 v4 = constanteMagique * vec4(1, 1, 1, 0.0);

    mat4 v = mat4(v1, v2, v3, v4);
    // Parcours le long du rayon avec pas variale (Sphere Tracing)
    for(; nbIters < 50; ++nbIters) {
        if(abs(dst) < prec * t) // Détection de contact (précision dépend de t)
            break;
        // test clipping limite de la caméra
        if(t > 20.0)
            break;
        // ajouter la mise a jour de la nouvelle distance sur le rayon
        // ... Ajouter la mise à jour de la nouvelle distance sur le rayon
        // ... Ajouter la mise à jour du point le long du rayon (déplacement)
        pt += dst * rd;
        t += dst;
        // ... Ajouter l'évaluation de la nouvelle position sur le rayon pour la boîte et les sphères
        dstB = dstCadre(pt, centreB, diagB, angleB, epaisseurCadre);
        dstS = dstSpheres(pt);
        // ... Récupérer la distance minimale dans dst
        // dst = min(dstS, dstB);
        dst = smin(dstS, dstB, rayonDeFusion);
    }

    // La couleur finale dépend du décor si le rayon ne touche aucun objet
    if(abs(dst) > prec * t) {
        fragColor = texture(iChannel0, rd);
        return;
    }
    // calcul de la normal au point pt
    for(int i = 0; i < 4; i++) {
        if(dst == dstB)
            normalObjet += vec3(v[i]) * dstCadre(pt + vec3(v[i]), centreB, diagB, angleB, epaisseurCadre);
        else
            normalObjet += vec3(v[i]) * dstSpheres(pt + vec3(v[i]));
    }
    // Sélection de la couleur selon l'objet en contact
    fragColor = vec4((dst == dstB) ? coulB : coulS, 1.0);

    // Coloriage selon la distance à la caméra
    if(coloriageDistance)
        // Intensité de la couleur selon la distance de l'objet à la caméra
        fragColor = vec4((1.0 - t / (2.0 * distanceCameraCible)) * fragColor.xyz, 1.0);
    if(coloriageIterations)
        // couleur en fonction du nombre d'itérations
        fragColor = vec4((float(nbIters) / 50.0) * vec3(1.0), 1.0);
    if(coloriageNormales)
        // couleur en fonction de la normale
        fragColor = vec4(normalize(normalObjet), 1.0);
    if(coloriageReflexion) {
        vec3 rf = reflect(rd, normalize(normalObjet));
        vec3 col = texture(iChannel0, rf).xyz;
        // melange en fonction du coeff de reflexion
        fragColor = mix(fragColor, vec4(col, 1.0), reflexion);
        // fragColor = vec4(reflect(rd, normalize(normalObjet)*texture(iChannel0,rd.xy).xyz), 1.0);
    }
    if(coloriageRefraction){
        vec3 rf = refract(rd,normalize(normalObjet),refraction);
        vec3 col = texture(iChannel0, rf).xyz;
        fragColor = mix(fragColor,vec4(col,1.0),transparence);
    }
    if(coloriageIlumination){;
        // lumiere finale = somme couleur ambiante + couleur diffuse
        vec3 lightDir = cross(normalize(normalObjet),normalize(lumiere));
        float diffuse = max(dot(normalize(normalObjet),normalize(lumiere-pt)),0.0);
        // specular
        vec3 viewDir = normalize(rd);
        vec3 reflectDir = reflect(-lightDir,normalize(normalObjet));
        float spec = pow(max(dot(viewDir,reflectDir),0.0),32.0);

        // couleur finale
        // fragColor = vec4((diffuse+ambiant.xyz*ambiant.w)*fragColor.xyz,1.0);
        fragColor = vec4((diffuse+ambiant.xyz*ambiant.w+spec)*fragColor.xyz,1.0);
        vec3 col = vec4(diffuse*fragColor.xyz,1.0).xyz;
    }
}
