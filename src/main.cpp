#include <iostream>
#include "dosmTensor.hpp"

using namespace dosm;

// ================= PRINT =================

template<idx_t N>
void print_vec(const tensor<double,N>& v, const char* name)
{
    std::cout << name << " = [ ";
    for (idx_t i=0;i<N;i++) std::cout << v(i) << " ";
    std::cout << "]\n";
}

template<idx_t I, idx_t J>
void print_mat(const tensor<double,I,J>& A, const char* name)
{
    std::cout << name << ":\n";
    for (idx_t i=0;i<I;i++)
    {
        for (idx_t j=0;j<J;j++)
            std::cout << A(i,j) << " ";
        std::cout << "\n";
    }
    std::cout << "\n";
}

// ================= MAIN =================

int main()
{
    // ================= VECTEURS =================
    tensor<double,3> v, w;
    v(0)=3; v(1)=4; v(2)=0;
    w(0)=1; w(1)=2; w(2)=3;

    print_vec(v, "v");
    print_vec(w, "w");

    // produit scalaire (selon TON implémentation)
    auto vw = v * w;
    std::cout << "v · w = " << vw.data[0] << "\n";

    std::cout << "||v|| = " << v.norm() << " (attendu 5)\n";

    auto vn = v.normalize();
    std::cout << "||normalize(v)|| = " << vn.norm() << "\n\n";

    // ================= MATRICES =================
    tensor<double,2,2> A, B;

    A(0,0)=1; A(0,1)=2;
    A(1,0)=3; A(1,1)=4;

    B(0,0)=10; B(0,1)=20;
    B(1,0)=30; B(1,1)=40;

    print_mat(A, "A");
    print_mat(B, "B");

    print_mat(A + B, "A + B");
    print_mat(B - A, "B - A");

    print_mat(A * B, "A * B (produit matriciel)");

    auto ABs = A * B; // déjà affiché

    auto AA = A * A;
    print_mat(AA, "A * A");

    std::cout << "<A,A> = " << A.norm() * A.norm()
          << " (produit scalaire Frobenius)\n";

    std::cout << "||A|| = " << A.norm() << "\n";

    auto An = A.normalize();
    std::cout << "||normalize(A)|| = " << An.norm() << "\n\n";

    // ================= TENSORIEL =================
    auto T = A | B;

    std::cout << "Produit tensoriel:\n";
    std::cout << "T(1,0,0,1) = "
              << T(1,0,0,1)
              << " (attendu 3*20=60)\n";

    return 0;
}
