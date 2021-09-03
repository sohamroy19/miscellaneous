/**
 * Author   :   Soham Roy EE20B130
 * Date     :   11-03-2021
 * Purpose  :   To solve thetaRate (Landau-Lifshitz-Gilbert equation) with
 *              Euler, Heun's, Midpoint, RK3, RK4, and RK45 methods.
 * Inputs   :   alpha, step are command-line arguments
 * Outputs  :   euler.txt, heun.txt, midpoint.txt, RK3.txt, RK4.txt, RK45.txt:
 *              output puts in cartesian coordinates
 */

#include <math.h>   // standard math library
#include <stdio.h>  // standard i/o library
#include <stdlib.h> // stadard utility library

#define PI 3.14159265358979323846
#define absolute(x) ((x < 0) ? -x : x)

/* The ODE is defined here as 2 macros */
#define phiRate ((1.76e11 / (alpha * alpha + 1)) * (100e-3 / 1.256637e-6))
#define thetaRate(th) (dt * phiRate * alpha * sin(th))

/* initial values for the ODE */
#define theta_in (179 * PI / 180) // 179 degrees
#define phi_in (1 * PI / 180)     //   1 degree

/* This struct is for making a linked list (unknown length) */
typedef struct List {
    double theta;
    struct List *next; // points to the next element
} List;

void output(FILE *fp, double theta, double phi);
double euler(double dt, double theta, double phi, double alpha, List *head);
double heun(double dt, double theta, double phi, double alpha, List *head);
double midpoint(double dt, double theta, double phi, double alpha, List *head);
double RK3(double dt, double theta, double phi, double alpha, List *head);
double RK4(double dt, double theta, double phi, double alpha, List *head);
List *RK45(double dt, double theta, double phi, double alpha);

/* Purpose  :   This is the main function, it calls the other functions
 *              as required.
 * Inputs   :   alpha: from command-line, used in ODE formula
 *              step: from command-line, time intervals we consider
 * Outputs  :   prints the values of errors for the three methods
 */
int main(int argc, char **argv) {
    double alpha, step;
    sscanf(argv[1], "%lf", &alpha);
    sscanf(argv[2], "%lf", &step);

    List *head = RK45(step, theta_in, phi_in, alpha); // list with 'correct' values
    printf("Error in Euler's :\t%f\n", euler(step, theta_in, phi_in, alpha, head));
    printf("Error in Heun's  :\t%f\n", heun(step, theta_in, phi_in, alpha, head));
    printf("Error in Midpoint:\t%f\n", midpoint(step, theta_in, phi_in, alpha, head));
    printf("Error in RK3     :\t%f\n", RK3(step, theta_in, phi_in, alpha, head));
    printf("Error in RK4     :\t%f\n", RK4(step, theta_in, phi_in, alpha, head));

    free(head);
    return 0;
}

/* Purpose  :   Outputs the points after converting to cartesian
 * Inputs   :   fp: points to the file to be written to
 *              theta, phi: denotes the point, along with r=1
 * Outputs  :   converts to cartesian and outputs to file at fp
 */
void output(FILE *fp, double theta, double phi) {
    fprintf(fp, "%f %f %f\n", sin(theta) * sin(phi), sin(theta) * cos(phi), cos(theta));
}

/* Purpose  :   Solves the ODE with RK45 method
 * Inputs   :   dt: time interval to be taken at each iteration
 *              theta: initialised to initial value of theta
 *              phi: initialised to initial value of phi
 *              alpha: value to be used in ODE
 * Outputs  :   prints to console the switching time
 *              RK45.txt: coordinates of each point
 *              returns a linked list for later error calculation
 */
List *RK45(double dt, double theta, double phi, double alpha) {
    FILE *fp = fopen("RK45.txt", "w");
    int count = 0;
    List *head = malloc(sizeof(List)), *current, *temp;
    current = head; // first node is a dummy

    while (theta > 1 * PI / 180) {   // 1 degree
        temp = malloc(sizeof(List)); // allocate temp
        temp->theta = theta;
        current->next = temp;
        current = temp; // step ahead
        output(fp, theta, phi);

        double hk1 = thetaRate(theta);
        double hk2 = thetaRate(theta + 2.0 * hk1 / 9.0);
        double hk3 = thetaRate(theta + 1.0 * hk1 / 12. + 1.0 * hk2 / 4);
        double hk4 = thetaRate(theta + 69. * hk1 / 128 - 243 * hk2 / 128 + 135 * hk3 / 64);
        double hk5 = thetaRate(theta - 17. * hk1 / 12. + 27. * hk2 / 4.0 - 27. * hk3 / 5. + 16. * hk4 / 5.);
        double hk6 = thetaRate(theta + 65. * hk1 / 432 - 5.0 * hk2 / 16. + 13. * hk3 / 16 + 4.0 * hk4 / 17 + 5.0 * hk5 / 144);
        phi += dt * phiRate;
        theta += (47 * hk1 + 24 * 9 * hk3 + 64 * hk4 + 15 * hk5 + 6 * 18 * hk6) / 450;
        count++;
    }
    printf("Switching Time   :\t%e\n\n", dt * count / 2);

    fclose(fp);
    return head;
}

/* Purpose  :   Solves the ODE with euler's method
 * Inputs   :   dt: time interval to be taken at each iteration
 *              theta: initialised to initial value of theta
 *              phi: initialised to initial value of phi
 *              alpha: value to be used in ODE
 *              head: head of the linked list of 'correct' values
 * Outputs  :   euler.txt: coordinates of each point
 *              returns rms error from RK45 values
 */
double euler(double dt, double theta, double phi, double alpha, List *head) {
    FILE *fp = fopen("euler.txt", "w");
    double stdev = 0;
    int count = 0;

    while (theta > 1 * PI / 180) {
        head = head->next; // traverse to next element
        fprintf(fp, "%f\n", theta - head->theta);

        double hk1 = thetaRate(theta);
        phi += dt * phiRate;
        theta += hk1;
        stdev += (theta - head->theta) * (theta - head->theta);
        count++;
    }
    fclose(fp);
    return sqrt(stdev / count);
}

/* Purpose  :   Solves the ODE with heun's method
 * Inputs   :   dt: time interval to be taken at each iteration
 *              theta: initialised to initial value of theta
 *              phi: initialised to initial value of phi
 *              alpha: value to be used in ODE
 *              head: head of the linked list of 'correct' values
 * Outputs  :   heun.txt: coordinates of each point
 *              returns rms error from RK45 values
 */
double heun(double dt, double theta, double phi, double alpha, List *head) {
    FILE *fp = fopen("heun.txt", "w");
    double stdev = 0;
    int count = 0;

    while (theta > 1 * PI / 180) {
        head = head->next; // traverse to next element
        fprintf(fp, "%f\n", theta - head->theta);

        double hk1 = thetaRate(theta);
        double hk2 = thetaRate(theta + 1.0 * hk1);
        phi += dt * phiRate;
        theta += (hk1 + hk2) / 2;
        stdev += (theta - head->theta) * (theta - head->theta);
        count++;
    }
    fclose(fp);
    return sqrt(stdev / count);
}

/* Purpose  :   Solves the ODE with midpoint method
 * Inputs   :   dt: time interval to be taken at each iteration
 *              theta: initialised to initial value of theta
 *              phi: initialised to initial value of phi
 *              alpha: value to be used in ODE
 *              head: head of the linked list of 'correct' values
 * Outputs  :   midpoint.txt: coordinates of each point
 *              returns rms error from RK45 values
 */
double midpoint(double dt, double theta, double phi, double alpha, List *head) {
    FILE *fp = fopen("midpoint.txt", "w");
    double stdev = 0;
    int count = 0;

    while (theta > 1 * PI / 180) {
        head = head->next; // traverse to next element
        fprintf(fp, "%f\n", theta - head->theta);

        double hk1 = thetaRate(theta);
        double hk2 = thetaRate(theta + 0.5 * hk1);
        phi += dt * phiRate;
        theta += hk2;
        stdev += (theta - head->theta) * (theta - head->theta);
        count++;
    }
    fclose(fp);
    return sqrt(stdev / count);
}

/* Purpose  :   Solves the ODE with RK3 method
 * Inputs   :   dt: time interval to be taken at each iteration
 *              theta: initialised to initial value of theta
 *              phi: initialised to initial value of phi
 *              alpha: value to be used in ODE
 *              head: head of the linked list of 'correct' values
 * Outputs  :   RK3.txt: coordinates of each point
 *              returns rms error from RK45 values
 */
double RK3(double dt, double theta, double phi, double alpha, List *head) {
    FILE *fp = fopen("RK3.txt", "w");
    double stdev = 0;
    int count = 0;

    while (theta > 1 * PI / 180) {
        head = head->next; // traverse to next element
        fprintf(fp, "%f\n", theta - head->theta);

        double hk1 = thetaRate(theta);
        double hk2 = thetaRate(theta + 0.5 * hk1);
        double hk3 = thetaRate(theta + 2.0 * hk2 - hk1);
        phi += dt * phiRate;
        theta += (hk1 + 4 * hk2 + hk3) / 6;
        stdev += (theta - head->theta) * (theta - head->theta);
        count++;
    }
    fclose(fp);
    return sqrt(stdev / count);
}

/* Purpose  :   Solves the ODE with RK4 method
 * Inputs   :   dt: time interval to be taken at each iteration
 *              theta: initialised to initial value of theta
 *              phi: initialised to initial value of phi
 *              alpha: value to be used in ODE
 *              head: head of the linked list of 'correct' values
 * Outputs  :   RK4.txt: coordinates of each point
 *              returns rms error from RK45 values
 */
double RK4(double dt, double theta, double phi, double alpha, List *head) {
    FILE *fp = fopen("RK4.txt", "w");
    double stdev = 0;
    int count = 0;

    while (theta > 1 * PI / 180) {
        head = head->next; // traverse to next element
        fprintf(fp, "%f\n", theta - head->theta);

        double hk1 = thetaRate(theta);
        double hk2 = thetaRate(theta + 0.5 * hk1);
        double hk3 = thetaRate(theta + 0.5 * hk2);
        double hk4 = thetaRate(theta + hk3);
        phi += dt * phiRate;
        theta += (hk1 + 2 * hk2 + 2 * hk3 + hk4) / 6;
        stdev += (theta - head->theta) * (theta - head->theta);
        count++;
    }
    fclose(fp);
    return sqrt(stdev / count);
}
