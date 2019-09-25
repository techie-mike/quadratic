#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

//--switching-on UnitTest--//
const int switchTEST = 1;
//  If "0" - OFF
//  If "1" - ON
//-------------------------//

const int NORESULT   = 0;
const int ONERESULT  = 1;
const int TWORESULT  = 2;
const int UNLIMRESULT= 3;
const int TRUE       = 1;
const int FALSE      = 0;


int SolveSquare(double a, double b, double c, double * x1, double *x2);
int SolveLinearA(double b, double c, double *x1);
void PrintResults(int sum, double x1, double x2);
void UnitTest();
int IsZero(double);
double *Input_calloc(const char text[], int amount);

int main()
{
    double x1 = NAN, x2 = NAN;
    printf("Program for solving the quadratic equation.\n");
    printf("Michail Kaskov 05.09.2019\n");
    if (switchTEST){
        UnitTest();
        return 0;
    }
    double *data = Input_calloc("Input a, b, c:", 3);
    assert(data!=0);
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
double *Input_calloc(const char text[], const int amount){
    printf(text);
    double *data = (double*) calloc(amount, sizeof(data[0]));
    for (int i=0; i< amount; i++){
        if (!scanf("%lg", &data[i])) {
            i--;
            fflush(stdin);
        }
        assert(isfinite(data[i]));
    }
    return data;
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

    if (IsZero(a)) return SolveLinearA(b, c, x1);
    if (IsZero(b)) {
        if (c==0){
            *x1 = 0;
            return ONERESULT;
            }
            else{
                //printf("%lg %lg", c, a);
                *x1 = sqrt(-c/a);
                *x2 =- *x1;
                assert(isfinite(*x1));
            return TWORESULT;
        }
    }
    if (IsZero(c)){
        *x1 = 0;
        SolveLinearA(a, b, x2);
        assert(isfinite(*x2));
        return TWORESULT;
    }

    double d = b*b - 4.0*a*c;

    if (d < 0) return NORESULT;
    if (d == 0) {
        *x1=-b/(2*a);
        return ONERESULT;
    }
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
int SolveLinearA(double b, double c, double *x1){
    if (c == 0) return UNLIMRESULT;
    else {
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
        printf("NO RESULTS");
        break;
    case ONERESULT:
        printf("One root: %lg", x1);
        break;
    case TWORESULT:
        printf("Two roots: %lg %lg", x1, x2);
        break;
    case UNLIMRESULT:
        printf("UNLIMITED RESULTS");
        break;
    }
}


/** \brief check on on "zero" of double
 *
 * \param value double verifiable value
 * \return int TRUE if "zero" else FALSE
 *
 */
int IsZero(double value){
    if (fabs(value) <= 1e-7) return TRUE;
    else return FALSE;
}


void UnitTest(){
    printf("TEST\n");
    const int nTest = 4;
    //  Number of arrayTEST[][i]
    //  i  =  | 0  1  2  3   4   5
    //  mean  | a  b  c  x1  x2  sum
   /* double arrayTEST[nTest][6]{
        1,  6,  9, -3,  0, 1,
        0,  1,  3, -3,  0, 1,
        1,  0, -9,  3, -3, 2,
        1, -5,  0,  0,  5, 2
    }; */
    struct TEST {
        double a;
        double b;
        double c;
        double x1;
        double x2;
        double sum;

    } arrayTEST[nTest] = {
        1,  6,  9, -3,  0, 1,
        0,  1,  3, -3,  0, 1,
        1,  0, -9,  3, -3, 2,
        1, -5,  0,  0,  5, 2
    };

    for (int i = 0; i<nTest; i++){
        double x1 = NAN, x2 = NAN;
        double *data = (double*) calloc(3, sizeof(data[0]));
//        for (int j = 0; j < 3; j++){
//            data[j] = arrayTEST[i];
//        }
        data[0] = arrayTEST[i].a;
        data[1] = arrayTEST[i].b;
        data[2] = arrayTEST[i].c;

        int sum = SolveSquare(data[0], data[1], data[2], &x1, &x2);
        free(data);

        //-----verification------//

        if (sum != arrayTEST[i].sum){
            printf("Error on %d test:\nprogram: %d  test:  %lg\n", i, sum, arrayTEST[i].sum);
            assert(IsZero(sum - arrayTEST[i].sum));
        };
        if (sum == 1){
            if (!IsZero(x1 - arrayTEST[i].x1)){
                printf("Error on %d test:\nprogram: %lg  test:  %lg\n", i, x1, arrayTEST[i].x1);
                assert(IsZero(x1 - arrayTEST[i].x1));
            }
        }
        if (sum == 2){
            if (!IsZero(x1 - arrayTEST[i].x1)){
                printf("Error on %d test:\nprogram: %lg  test:  %lg\n", i, x1, arrayTEST[i].x1);
                assert(IsZero(x1 - arrayTEST[i].x1));
            }
            if (!IsZero(x2 - arrayTEST[i].x2)){
                printf("Error on %d test:\nprogram: %lg  test:  %lg\n", i, x2, arrayTEST[i].x2);
                assert(IsZero(x2 - arrayTEST[i].x2));
            }
        }
       /* if (sum != arrayTEST[i][5] || sum == 1 || sum == 2){
            printf("Error on %d test:\nprogram: %d  test:  %lg\n", i, sum, arrayTEST[i][5]);
            assert(IsZero(sum - arrayTEST[i][5]));
        } */
    }
    printf("test : OK");
}
