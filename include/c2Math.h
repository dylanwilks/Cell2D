#ifndef C2MATH_H_INCLUDED
#define C2MATH_H_INCLUDED

#include <math.h>
#include <bitset>
#include <vector>
#include <string>
#include <iostream>

struct c2Vector
{
    //Construct empty vector
    c2Vector() {}

    //Create vector with given parameters
    c2Vector(float x, float y): x(x), y(y) {}

    //Annihilate the vector components
    void SetZero()
    {
        x = 0;
        y = 0;
    }

    //Set vector values
    void Set(float x_, float y_)
    {
        x = x_;
        y = y_;
    }

    //Inverts the elements
    c2Vector operator -() const
    {
        c2Vector v;
        v.x = -x;
        v.y = -y;     

        return v;
    }

    //Add 2 vectors, similar to Obj1.Add(Obj2)
    c2Vector operator + (const c2Vector& v) const
    {
        c2Vector obj;
        obj.x = this->x + v.x;
        obj.y = this->y + v.y;

        return obj;
    }

    //Subtracts 2 vectors
    c2Vector operator - (const c2Vector& v) const
    {                                                
        c2Vector obj;                                
        obj.x = this->x - v.x;                       
        obj.y = this->y - v.y;                       

        return obj;
    }

    //Add components of another vector onto this one
    void operator += (const c2Vector& v)
    {
        x += v.x;
        y += v.y;
    }

    //Subtract 2 vectors
    void operator -= (const c2Vector& v)
    {
        x -= v.x;
        y -= v.y;
    }

    //Scale a vector
    void operator *= (float a)
    {
        x *= a;
        y *= a;
    }

    bool operator < (const c2Vector& v) const
    {
        return x < v.x && y < v.y;
    }

    bool operator <= (const c2Vector& v) const
    {
        return x <= v.x && y <= v.y;
    }

    bool operator > (const c2Vector& v) const
    {
        return x > v.x && y > v.y;
    }

    bool operator >= (const c2Vector& v) const
    {
        return x >= v.x && y >= v.y;
    }

    //Get length of the vector
    float Magnitude() const
    {
        return sqrtf(x * x + y * y);
    }

    //Returns normalized version of vector (for direction)
    c2Vector Normalize() const
    {
        c2Vector v;
        float mag = this->Magnitude();
        float invMag = 1.0 / mag;
        v *= invMag;

        return v;
    }

    //Returns the floor of the components
    c2Vector Floor() const
    {
        c2Vector m_v;
        m_v.Set(floor(this->x), floor(this->y));

        return m_v;
    }

    //Vector coordinates
    float x, y;
};

//For printing vectors
inline std::ostream& operator << (std::ostream& out, const c2Vector& vector) 
{
    return out << "(" << vector.x << ", " << vector.y << ")";
}

//Add a fixed value to both components of the vector then return another
inline c2Vector operator + (const c2Vector& v, float f) 
{
    return c2Vector(v.x + f, v.y + f);
}

//Subtract both components with a fixed value then return another
inline c2Vector operator - (const c2Vector& v, float f) 
{
    return c2Vector(v.x - f, v.y - f);
}

//Scale the vector then return another
inline c2Vector operator * (const c2Vector& v, float f) 
{
    return c2Vector(v.x * f, v.y * f);
}

//Scale the vector (division) then return another
inline c2Vector operator / (const c2Vector& v, float f) 
{
    return c2Vector(v.x / f, v.y / f);
}

//Determines if 2 vectors are unequal
inline bool operator != (const c2Vector& v1, const c2Vector& v2) 
{
    if(v1.x != v2.x || v1.y != v2.y) { return true; }
    else { return false; }
}

//Determines if 2 vectors are equal
inline bool operator == (const c2Vector& v1, const c2Vector& v2) 
{
    if(v1.x == v2.x && v1.y == v2.y) { return true; }
    else { return false; }
}

struct c2Matrix22
{
    //Construct an empty 2x2 matrix
    c2Matrix22() {}

    //Construct a 2x2 matrix with corresponding values
    c2Matrix22(float a11, float a12, float a21, float a22)
    {
        ex.x = a11; ex.y = a21;
        ey.x = a12; ey.y = a22;
    }

    //Add another matrix on
    c2Matrix22 operator + (const c2Matrix22& mat1) const
    {
        c2Matrix22 mat;
        mat.Set(this->ex + mat1.ex,
                this->ey + mat1.ey);

        return mat;
    }

    //Substract from another matrix
    c2Matrix22 operator - (const c2Matrix22& mat1) const
    {
        c2Matrix22 mat;
        mat.Set(this->ex - mat1.ex,
                this->ey - mat1.ey);

        return mat;
    }

    //Scale the matrix by a fixed value
    c2Matrix22 operator * (float val) const
    {
        c2Matrix22 mat;

        mat.Set(this->ex * val,
                this->ey * val);

        return mat;
    }

    //Modify matrix components
    void Set(const c2Vector& c1, const c2Vector& c2)
    {
        ex = c1;
        ey = c2;
    }

    //Change floato identity matrix
    void SetIdentity()
    {
        ex.x = 1; ex.y = 0;
        ey.x = 0; ey.y = 1;
    }

    //Set components to zero; sets vectors to origin
    void SetZero()
    {
        ex.x = 0; ex.y = 0;
        ey.x = 0; ey.y = 0;
    }

    //Return the determinant of the matrix
    float det() const
    {
        return ex.x * ey.y - ey.x * ex.y;
    }

    //Return the inverse of the matrix
    c2Matrix22 GetInverse() const
    {
        c2Matrix22 mat;

        if(det() == 0) 
        {
            mat.SetZero();
            return mat;
        }

        c2Vector v1;
        v1.Set(ey.y, -ex.y);

        c2Vector v2;
        v2.Set(-ey.x, ex.x);

        mat.Set(v1, v2);
        mat = mat * det();

        return mat;
    }

    c2Vector Solve(const c2Vector& v) const
    {
        c2Vector product;
        product.Set(ex.x * v.x + ey.x * v.y,
                    ex.y * v.x + ey.y * v.y);

        return product;
    }

    //Column vectors (components) of the matrix
    c2Vector ex, ey;
};

//Add a fixed constant to all the elements
inline c2Matrix22 operator + (const c2Matrix22& mat1, float val) 
{
    c2Matrix22 mat;
    mat.Set(mat1.ex + val,
            mat1.ey + val);

    return mat;
}

//Subtract a fixed constant to all the elements
inline c2Matrix22 operator - (const c2Matrix22& mat1, float val) 
{
    c2Matrix22 mat;
    mat.Set(mat1.ex - val,
            mat1.ey - val);

    return mat;
}

//Scale a matrix by a constant
inline c2Matrix22 operator * (const c2Matrix22& mat1, float val) 
{
    c2Matrix22 mat;
    mat.Set(mat1.ex * val,
            mat1.ey * val);

    return mat;
}

//Divide the elements of a matrix by a constant
inline c2Matrix22 operator / (const c2Matrix22& mat1, float val) 
{
    c2Matrix22 mat;
    mat.Set(mat1.ex / val,
            mat1.ey / val);

    return mat;
}

//N bit element array
template <size_t N, size_t S>
class nBit
{
    typedef typename std::bitset<N*S>::reference bit_ref;

    public:
        nBit<N,S>(std::vector<bit_ref> bits) : bits(bits) {}

        nBit& operator=(int n) 
        {
            for(int i = 0; i < N; i++)
                bits[N-i-1] = (n >> i) & 0x1;
        }

        bool operator==(int n) 
        {
            for(int i = 0; i < N; i++)
                if((((n >> i) & 0x1) ^ bits[N-i-1]))
                    return false;

            return true;
        }

        operator int() const
        {
            int bit_seq = 0x0;
            for(int i = 0; i < N; i++) {
                bit_seq = bit_seq << 1;

                if(bits[i] == 1)
                    bit_seq = bit_seq | 0x1;
            }

            return bit_seq;
        }

        friend std::ostream& operator<<(std::ostream& out, const nBit<N,S>& seg)
        {
            std::string bitstring;
            for(int i = 0; i < N; i++)
                bitstring += std::to_string(seg.bits[i]);

            return out << bitstring << '\n';
        }

    private:
        std::vector<bit_ref> bits;
};

template <size_t N, size_t S>
class nBitArray
{
    typedef typename std::bitset<N*S>::reference bit_ref;

    public:
        nBit<N,S> operator[](int n) 
        {
            std::vector<bit_ref> bits;
            for(int i = 0; i < N; i++) 
                bits.push_back(bit_arr[N*n + i]);

            return nBit<N,S>(bits);
        }

        operator int() const
        {
            return static_cast<int>(bit_arr.to_ulong());
        }

        friend std::ostream& operator<<(std::ostream& out, const nBitArray<N,S>& bits)
        {
            std::string bitstring;
            for(int i = 0; i < N*S; i++)
                bitstring += std::to_string(bits.bit_arr[i]);

            return out << bitstring << '\n';
        }

    private:
        std::bitset<N*S> bit_arr;
};

#endif // C2MATH_H_INCLUDED
