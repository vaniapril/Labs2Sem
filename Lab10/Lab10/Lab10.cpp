#include <iostream>

struct Vector2D {
    double X;
    double Y;
};
struct Vector3D {
    long long X;
    long long Y;
    long long Z;
};

extern "C" double LengthVector2D(Vector2D &vector);

extern "C" Vector3D* SumVector3D(Vector3D & v1, Vector3D & v2);

void task1() {
    Vector2D vector = { 4.0, 3.0 };
    double length = LengthVector2D(vector);
    std::cout<<"Length: " <<length <<'\n';
}

void task2() {
    Vector3D vector1 = { 4, 3, 5 };
    Vector3D vector2 = { 5, 6, 7};
    Vector3D* _vector3 = SumVector3D(vector1, vector2);
    Vector3D vector3 = *_vector3;

    std::cout<<"Sum: (" << vector3.X << ", " << vector3.Y << ", " << vector3.Z << ")" <<'\n';
}

int main()
{
    task1();
    task2();
}
