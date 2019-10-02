#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

//--uncomment this #define if want turn on UnitTest--//

//#define switchTEST

//-------------------------//

const int NORESULT   = 0;
const int ONERESULT  = 1;
const int TWORESULT  = 2;
const int UNLIMRESULT= 3;
const int TRUE       = 1;
const int FALSE      = 0;

const double DEFAULT_ERROR = 1e-10;


int SolveSquare(double a, double b, double c, double * x1, double *x2);
int SolveLinearA(double b, double c, double *x1, double full_error);
void PrintResults(int sum, double x1, double x2);
void UnitTest();
int IsZero(double value, double full_error);
double *InputCalloc(const char text[], int amount);
double MaxError(double a, double b, double c);


int main()
{
    double x1 = NAN, x2 = NAN;
    printf("Program for solving the quadratic equation.\n");
    printf("Michail Kaskov 05.09.2019\n");

    #ifdef switchTEST
        UnitTest();
        return 0;
    #endif

    double *data = InputCalloc("Input a, b, c:", 3);
    assert(data != 0);
    int sum = SolveSquare(data[0], data[1], data[2], &x1, &x2);
    free(data);
    PrintResults(sum, x1, x2);

    return 0;
}


/** \brief input function
 *
 * \param text[] const char it is text which will be write before read number
 * \param amount const int it is amount of variables
 * \return double* pointer on array of variables
 *
 */
double *InputCalloc(const char text[], const int amount){
    assert(text != 0);
    assert(amount > 0);

    printf("%s", text);
    double *data = (double*) calloc(amount, sizeof(data[0]));
    assert(data != 0);
    for (int i = 0; i < amount; i++){
        if (!scanf("%lg", &data[i])) {
            i--;
            fflush(stdin);
        }
        assert(isfinite(data[i]));
    }
    return data;
}

/** \brief maximum error calculation
 *
 * \param a double input a
 * \param b double input b
 * \param c double input c
 * \return double size of error
 *
 */
double MaxError(double a, double b, double c){
    assert(isfinite(a));
    assert(isfinite(b));
    assert(isfinite(c));

    int max_pow = log10(a);

    int temp = log10(b);
    if (max_pow < temp){
        max_pow = temp;
    }

    temp = log10(c);
    if (max_pow < temp){
        max_pow = temp;
    }
    //max_pow = log10(10, max_pow);
    return DEFAULT_ERROR * max_pow;
}

/** \brief quadratic solution
 *
 * \param a double coefficient before x^2
 * \param b double coefficient before x
 * \param c double coefficient without x
 * \param x1 double* pointer on first root
 * \param x2 double* pointer on second root
 * \return int how many roots
 *
 */
int SolveSquare(double a, double b, double c, double *x1, double *x2){
    /////////////////////////
    // a*x^2 + b*x + c = 0 //
    /////////////////////////
    assert(x1 != nullptr);
    assert(x2 != nullptr);

    double full_error = MaxError(a,b,c);
    assert(isfinite(full_error));
    //printf("full_error = %lg\n", full_error);

    if (IsZero(a, full_error)) return SolveLinearA(b, c, x1, full_error);
    if (IsZero(b, full_error)) {
        if (IsZero(c, full_error)){
            *x1 = 0;
            return ONERESULT;
            }
            else{
		if (c > 0) return NORESULT;
                //printf("%lg %lg", c, a);
                *x1 = sqrt(-c/a);
                *x2 = -*x1;
                assert(isfinite(*x1));
            return TWORESULT;
        }
    }
    if (IsZero(c, full_error)){
        *x1 = 0;
        SolveLinearA(a, b, x2, full_error);
        assert(isfinite(*x2));
        return TWORESULT;
    }

    double d = b*b - 4.0*a*c;

    if (IsZero(d, full_error)) {
        *x1 = -b/(2*a);
        return ONERESULT;
    }

    if (d < 0) return NORESULT;

    d = sqrt(d);
    *x1 = (-b - d)/2/a;
    *x2 = (-b + d)/2/a;

    assert(isfinite(*x1));
    assert(isfinite(*x2));
    return TWORESULT;
    }


/** \brief quadratic solution if "a" == 0
 *
 * \param b double coefficient before x
 * \param c double coefficient without x
 * \param x1 double* pointer on first root
 * \return int how many roots
 *
 */
int SolveLinearA(double b, double c, double *x1, double full_error){
    assert(x1 != 0);

    if (IsZero(b, full_error)) return UNLIMRESULT;
    else {
	if (IsZero(c, full_error)) {
		*x1 = 0;
		return ONERESULT;
	}
        *x1 = -c/b;
        assert(isfinite(*x1));
        return ONERESULT;
    }
}


/** \brief print roots and quantity roots
 *
 * \param sum int quantity roots
 * \param x1 double first root
 * \param x2 double second root
 * \return void
 *
 */
void PrintResults(int sum, double x1, double x2){
    switch (sum) {
    case NORESULT:
        printf("NO RESULTS\n");
        break;
    case ONERESULT:
        printf("One root: %lg\n", x1);
        break;
    case TWORESULT:
        printf("Two roots: %lg %lg\n", x1, x2);
        break;
    case UNLIMRESULT:
        printf("UNLIMITED RESULTS\n");
        break;
    }
}


/** \brief check on on "zero" of double
 *
 * \param value double verifiable value
 * \return int TRUE if "zero" else FALSE
 *
 */
int IsZero(double value, double full_error){
    if (fabs(value) <= full_error) return TRUE;
    else return FALSE;
}

#ifdef switchTEST
void UnitTest(){
    printf("TEST\n");
    const int nTest = 4;

    struct TEST {
        double a;
        double b;
        double c;
        double x1;
        double x2;
        int sum;

    } arrayTEST[nTest] = {
        1,  6,  9, -3,  0, 1,
        0,  1,  3, -3,  0, 1,
        1,  0, -9,  3, -3, 2,
        1, -5,  0,  0,  5, 2
    };

    for (int i = 0; i<nTest; i++){
        double x1 = NAN, x2 = NAN;

        double full_error = 1e-10; //MaxError(arrayTEST[i].a, arrayTEST[i].b, arrayTEST[i].c);
        assert(isfinite(full_error));
        //printf("a = %lg b = %lg c = %lg\n", arrayTEST[i].a, arrayTEST[i].b, arrayTEST[i].c);
        int sum = (int)SolveSquare(arrayTEST[i].a, arrayTEST[i].b, arrayTEST[i].c, &x1, &x2);

        //-----verification------//

        if (sum != arrayTEST[i].sum){
            printf("Error on %d test:\nprogram: %d  test:  %d\n", i, sum, arrayTEST[i].sum);
            assert(IsZero(sum - arrayTEST[i].sum, full_error));
        }
        if (sum == 1){
            if (!IsZero(x1 - arrayTEST[i].x1, full_error)){
                printf("Error on %d test:\nprogram: %lg  test:  %lg\n", i, x1, arrayTEST[i].x1);
                assert(IsZero(x1 - arrayTEST[i].x1, full_error));
            }
        }

        if (sum == 2){
            if (!IsZero(x1 - arrayTEST[i].x1, full_error)){
                printf("Error on %d test:\nprogram: %lg  test:  %lg\n", i, x1, arrayTEST[i].x1);
                assert(IsZero(x1 - arrayTEST[i].x1, full_error));
            }
            if (!IsZero(x2 - arrayTEST[i].x2, full_error)){
                printf("Error on %d test:\nprogram: %lg  test:  %lg\n", i, x2, arrayTEST[i].x2);
                assert(IsZero(x2 - arrayTEST[i].x2, full_error));
            }
        }
        //printf("%d test - OK\n", i);
    }
    printf("test : OK\n");
}
#endif
