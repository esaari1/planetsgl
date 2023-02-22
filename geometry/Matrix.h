//============================================================================
//	Johns Hopkins University Whiting School of Engineering
//	605.761 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:  David W. Nesbitt
//	File:    Matrix.h
//	Purpose: Matrix class
//          Student should include "geometry.h" to get all 
//          class definitions included in proper order.
//
//============================================================================

#ifndef __MATRIX4x4_H__
#define __MATRIX4x4_H__

class Vector3D;
class Point3D;

// 4x4 matrix
class Matrix4x4
{
public:

	Matrix4x4(void);
   Matrix4x4::Matrix4x4(double e0, double e1, double e2, double e3,
   					double e4, double e5, double e6, double e7,
   					double e8, double e9, double e10, double e11,
                  double e12, double e13, double e14, double e15);
	Matrix4x4(const Matrix4x4& n);
   ~Matrix4x4(void) { }

	void SetIdentity();

	Matrix4x4& operator = (const Matrix4x4& n);
	bool operator == (const Matrix4x4& n);
   void Set(const double* m);
   double* Get() { return a; }

   // Read-only access functions
   double m11(void) const { return a[0];  }
	double m12(void) const { return a[4];  }
	double m13(void) const { return a[8];  }
	double m14(void) const { return a[12]; }
	double m21(void) const { return a[1];  }
	double m22(void) const { return a[5];  }
	double m23(void) const { return a[9];  }
	double m24(void) const { return a[13]; }
	double m31(void) const { return a[2];  }
	double m32(void) const { return a[6];  }
	double m33(void) const { return a[10]; }
	double m34(void) const { return a[14]; }
	double m41(void) const { return a[3];  }
	double m42(void) const { return a[7];  }
	double m43(void) const { return a[11]; }
	double m44(void) const { return a[15]; }
   double m(const unsigned int row, const unsigned int col) const;

   // Read-write access functions
   double& m11(void) { return a[0];  }
	double& m12(void) { return a[4];  }
	double& m13(void) { return a[8];  }
	double& m14(void) { return a[12]; }
	double& m21(void) { return a[1];  }
	double& m22(void) { return a[5];  }
	double& m23(void) { return a[9];  }
	double& m24(void) { return a[13]; }
	double& m31(void) { return a[2];  }
	double& m32(void) { return a[6];  }
	double& m33(void) { return a[10]; }
	double& m34(void) { return a[14]; }
	double& m41(void) { return a[3];  }
	double& m42(void) { return a[7];  }
	double& m43(void) { return a[11]; }
	double& m44(void) { return a[15]; }
   double& m(const unsigned int row, const unsigned int col);

	Matrix4x4 operator * (const Matrix4x4& n) const;
   Matrix4x4& operator *= (const Matrix4x4& n);
   Matrix4x4 & operator *= (const double s);
   Point3D operator *(const Point3D& v) const;
   Vector3D operator *(const Vector3D& v) const;
   Matrix4x4& Transpose();
   Matrix4x4 Inverse(void) const;
	void Translate(const double x, const double y, const double z);
	void Scale(const double x, const double y, const double z);
   void Rotate(const double angle, const double x, const double y, const double z);

private:
	double a[16];		// 4x4 matrix stored in column order (matches OpenGL)
};


#endif
