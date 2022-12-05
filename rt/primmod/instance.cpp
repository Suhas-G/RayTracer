#include <rt/primmod/instance.h>

namespace rt {

Instance::Instance(Primitive* content)
{
    this->contentPrimitive = content;
}

Primitive* Instance::content() {
    return this->contentPrimitive;
}

void Instance::reset() {
    this->transformation = Matrix::identity();
}

void Instance::translate(const Vector& t) {
    Matrix translationMatrix = Matrix::identity();
    translationMatrix[0][3] = t.x;
    translationMatrix[1][3] = t.y;
    translationMatrix[2][3] = t.z;
    this->transformation = rt::product(translationMatrix, this->transformation);
}

void Instance::rotate(const Vector& nnaxis, float angle) {
    Vector r = nnaxis.normalize();
    float rx = std::abs(r.x), ry = std::abs(r.y), rz = std::abs(r.z);
    Vector s = Vector::rep(0);
    if (rx <= ry && rx <= rz) {
        s.y = -r.z;
        s.z = r.y;
    } else if (ry <= rx && ry <= rz) {
        s.x = -r.z;
        s.z = r.x;
    } else {
        s.x = -r.y;
        s.y = r.x;
    }
    s = s.normalize();
    Vector t = rt::cross(r, s);
    Matrix m = Matrix::system(r, s, t);
    Matrix rotationX = Matrix::identity();
    float cosPhi = std::cos(angle), sinPhi = std::sin(angle);
    rotationX[1][1] = cosPhi;
    rotationX[1][2] = -sinPhi;
    rotationX[2][1] = sinPhi;
    rotationX[2][2] = cosPhi;
    this->transformation = rt::product(m,
                                rt::product(rotationX,
                                    rt::product(m.transpose(), this->transformation)));
}

void Instance::scale(float f) {
    Matrix scaleMatrix = Matrix::identity();
    scaleMatrix[0][0] = scaleMatrix[1][1] = scaleMatrix[2][2] = f;
    this->transformation = rt::product(scaleMatrix, this->transformation); 
}

void Instance::scale(const Vector& s) {
    Matrix scaleMatrix = Matrix::identity();
    scaleMatrix[0][0] = s.x;
    scaleMatrix[1][1] = s.y;
    scaleMatrix[2][2] = s.z;
    this->transformation = rt::product(scaleMatrix, this->transformation); 
}

void Instance::setMaterial(Material* m) {
    this->m = m;
}

void Instance::setCoordMapper(CoordMapper* cm) {
    this->cm = cm;
}

Intersection Instance::intersect(const Ray& ray, float tmin, float tmax) const {
    Ray transformedRay = ray.transform(this->transformation.invert());
    // TODO: Check if tmin, tmax has to be modified
    Intersection pIntersection = this->contentPrimitive->intersect(transformedRay, tmin, tmax);
    if (pIntersection) {
        Point transformedHitpoint = this->transformation * pIntersection.hitPoint();
        float originalDistance = ray.getDistance(transformedHitpoint);
        Vector transformedNormal = this->transformation * pIntersection.normal();
        return Intersection(originalDistance, ray, pIntersection.solid, transformedNormal, pIntersection.local());
    }

    return Intersection::failure();

}

BBox Instance::getBounds() const {
    BBox primBounds = this->contentPrimitive->getBounds();
    return BBox(this->transformation * primBounds.min, this->transformation * primBounds.max);
}

}