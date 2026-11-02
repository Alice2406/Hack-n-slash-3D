#include "cpe/navigation.hpp"
namespace cpe {
bool NavMesh::Build() {
    // TODO J3 : valider et structurer source_, calculer les adjacences/portails.
    // Remplir polygons_, portals_, graph_, puis définir built_=true.
    return false;
}

int NavMesh::Locate(Vec2 point) const {
    if (!Finite(point))
        return -1;

    for (int i = 0; i < polygons_.size(); i++)
    {
        if (Contains(polygons_[i], point))
            return i;
    }
    return -1;
}

bool NavMesh::Contains(Polygon polygon, Vec2 point) const {
    double side;
    bool positif = false;
    bool negatif = false;
    if (!Finite(point))
        throw std::_Xinvalid_argument;
    for (int i = 0; i < polygon.vertices.size(); i++)
    {
        side = Cross((polygon.vertices[(i + 1) % polygon.vertices.size()]) - polygon.vertices[i], point - polygon.vertices[i]);
        if (side < -1e-8)
            negatif = true;
        else if (side > 1e-8)
            positif = true;
    }
    return !(positif && negatif);
}

bool NavMesh::SegmentInside(Vec2 from, Vec2 to) const {
    // TODO J3 : vérifier qu'un segment reste dans la surface navigable.
    (void)from;
    (void)to;
    return false;
}
Path NavMesh::FindPath(Vec2 from, Vec2 to, Algorithm algorithm) const {
    // TODO J3 : relier localisation, recherche sur graphe et trajet géométrique.
    (void)from;
    (void)to;
    (void)algorithm;
    return {};
}
}
