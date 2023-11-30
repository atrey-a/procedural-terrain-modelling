#include <GL/glut.h>
#include <cmath>
#include <vector>
using namespace std;

// -------DEFINITIONS--------

// General land definition -
const int width = 500;
const int height = 500;

int defaultNoiseLimit = 1;
int defaultOctaves = 6;
double defaultPersistence = 1;
double defaultLacunarity = 0.3;
char defaultInterpolation = 'l';

// Pond definition -
int pond_center_x = 300;
int pond_center_y = 250;
int pond_radius = 50;

int pondNoiseLimit = 1;
int pondOctaves = 6;
double pondPersistence = 1;
double pondLacunarity = 0.3;
char pondInterpolation = 'l';

// Mountain definition -
int mountain_center_x = 100;
int mountain_center_y = 250;
int mountain_radius = 70;

int mountainNoiseLimit = 1;
int mountainOctaves = 6;
double mountainPersistence = 1;
double mountainLacunarity = 0.3;
char mountainInterpolation = 'l';

// River definition -
int river_x1 = 200;
int river_x2 = 200;
int river_y1 = 200;
int river_y2 = 200;

int riverNoiseLimit = 1;
int riverOctaves = 6;
double riverPersistence = 1;
double riverLacunarity = 0.3;
char riverInterpolation = 'l';

// ----------------------------

float eye_x = 550.0f;
float eye_y = 250.0f;
float eye_z = 100.0f;
float look_x = 0.0f;
float look_y = 250.0f;
float look_z = -200.0f;
float up_x = 0.0f;
float up_y = 0.0f;
float up_z = 50.0f;

/**
 * @brief A helper function that is used while panning/sliding in 'u' direction 
 * 
 * @param delU a parameter that signifies the amount of sliding to be done in 'u' direction
 */
void slide_u(float delU)
{
	double length = sqrt(pow(eye_x - look_x, 2) + pow(eye_y - look_y, 2) + pow(eye_z - look_z, 2));

    double n_x = (look_x - eye_x) / length;
    double n_y = (look_y - eye_y) / length;
    double n_z = (look_z - eye_z) / length;
    
    double u_x = up_y*n_z - up_z*n_y;
    double u_y = up_z*n_x - up_x*n_z;
    double u_z = up_x*n_y - up_y*n_x;

    eye_x += delU * u_x;
    eye_y += delU * u_y;
    eye_z += delU * u_z;
	
    look_x += delU * u_x;
    look_y += delU * u_y;
    look_z += delU * u_z;

    glutPostRedisplay();
}

/**
 * @brief A helper function that is used while panning/sliding in 'v' direction 
 * 
 * @param delV a parameter that signifies the amount of sliding to be done in 'v' direction
 */
void slide_v(float delV)
{
	double length = sqrt(pow(eye_x - look_x, 2) + pow(eye_y - look_y, 2) + pow(eye_z - look_z, 2));

    double n_x = (look_x - eye_x) / length;
    double n_y = (look_y - eye_y) / length;
    double n_z = (look_z - eye_z) / length;
    
    double u_x = up_y*n_z - up_z*n_y;
    double u_y = up_z*n_x - up_x*n_z;
    double u_z = up_x*n_y - up_y*n_x;

    double v_x = n_y*u_z - n_z*u_y;
    double v_y = n_z*u_x - n_x*u_z;
    double v_z = n_x*u_y - n_y*u_x;

    eye_x += delV * v_x;
    eye_y += delV * v_y;
    eye_z += delV * v_z;
	
    look_x += delV * v_x;
    look_y += delV * v_y;
    look_z += delV * v_z;

    glutPostRedisplay();
}

/**
 * @brief A helper function that is used while zooming in 'n' direction 
 * 
 * @param delN a parameter that signifies the amount of zooming to be done in 'n' direction
 */
void zoom(float delN)
{
	double length = sqrt(pow(eye_x - look_x, 2) + pow(eye_y - look_y, 2) + pow(eye_z - look_z, 2));

    double n_x = (look_x - eye_x) / length;
    double n_y = (look_y - eye_y) / length;
    double n_z = (look_z - eye_z) / length;
    
    double u_x = up_y*n_z - up_z*n_y;
    double u_y = up_z*n_x - up_x*n_z;
    double u_z = up_x*n_y - up_y*n_x;

    eye_x += delN * n_x;
    eye_y += delN * n_y;
    eye_z += delN * n_z;

    glutPostRedisplay();
}

/**
 * @brief A function that resets to the initial view, wrt to all three characteristics: eye and look coordinates and the up vector  
 * 
 */
void reset () {
    eye_x = 550.0f;
    eye_y = 250.0f;
    eye_z = 100.0f;
    look_x = 0.0f;
    look_y = 250.0f;
    look_z = -200.0f;
    up_x = 0.0f;
    up_y = 0.0f;
    up_z = 50.0f;

    glutPostRedisplay();
}

/**
 * @brief A function that rotates the camera about the direction of 'v' vector
 * 
 * @param angle a parameter that signifies the angle (in degrees) to be yawed
 */
void yaw (float angle)
{
    float cs = cos(M_PI/180 * angle); 
    float sn = sin(M_PI/180 * angle);

    up_x = cs*up_x + sn*up_z;
    up_z = -sn*up_x + cs*up_z;

    look_x = cs*look_x + sn*look_z;
    look_z = -sn*look_x + cs*look_z;

    glutPostRedisplay();
}

// Function to generate Perlin noise in 3D
double noise(int x, int y, int z, int noiseLimit) {
    int n = x + y * 57 + z * 131;
    n = (n << 20) ^ n;
    return (1.0 - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0)*noiseLimit;
}

// Function to interpolate between two values
double interpolate(double a, double b, double t, char f) {
    if (f == 's') // Sin-Cosine Interpolation
        return a * cos(t) + b * sin(t);
    else if (f == 'c') // Circular Interpolation
        return a * cos(t) * cos(t) + b * sin(t) * sin(t);
    else if (f == 'p') // Parabolic Interpolation
        return a * (1 - t * t) + b * t;
    else // Linear Interpolation
        return a * (1.0 - t) + b * t;
}

// Function to generate 3D Perlin noise
double perlinCalculate(int x, int y, int z, int noiseLimit, char interpolationFunc) {
    double n = noise(x, y, z, noiseLimit);
    double i1 = interpolate(noise(x - 1, y, z, noiseLimit), noise(x + 1, y, z, noiseLimit), n, interpolationFunc);
    double i2 = interpolate(noise(x, y - 1, z, noiseLimit), noise(x, y + 1, z, noiseLimit), n, interpolationFunc);
    double i3 = interpolate(noise(x, y, z - 1, noiseLimit), noise(x, y, z + 1, noiseLimit), n, interpolationFunc);
    return interpolate(interpolate(i1, i2, n, interpolationFunc), i3, n, interpolationFunc);
}

double perlin(int x, int y, int z, int noiseLimit, int octaves, double persistence, double lacunarity, char interpolationFunc){
    double total = 0.0;
    double frequency = 2;
    double amplitude = 1;

    for (int octave = 0; octave < octaves; ++octave) {
        total += amplitude * perlinCalculate(x * frequency, y * frequency, z * frequency, noiseLimit, interpolationFunc);
        frequency *= lacunarity;
        amplitude *= persistence;
    }

    // Normalize the result to the range [0, 1]
    return total;
}

void setColor(double perlinValue) {
    double r, g, b;
    if (perlinValue > defaultNoiseLimit*10*0.75) { // dark brown
        r = 0.341;
        g = 0.224;
        b = 0.051;
    }
    else if(perlinValue > defaultNoiseLimit*8*0.6){  // light brown
        r = 0.702;
        g = 0.459;
        b = 0.114;
    }
    else if(perlinValue > defaultNoiseLimit*2*0.4){  // dark green
        r = 0.251;
        g = 0.439;
        b = 0.059;
    }
    else if(perlinValue > defaultNoiseLimit*0.2){  // light green
        r = 0.455;
        g = 0.812;
        b = 0.082;
    }
    else if(perlinValue > defaultNoiseLimit*0.01){  // even lighter green
        r = 0.0;
        g = 1;
        b = 0.372;
    }
    else if(perlinValue > 0){  // even lighter green
        r = 0.0;
        g = 1;
        b = 0.372;
    } 
    else {    //blue
        r = 0.078;
        g = 0.604;
        b = 0.82;
        // r = 0.0;
        // g = 1;
        // b = 0.372;
    }

    glColor3d(r,g,b);
}

bool pond(int x, int y) {
    return ((x - pond_center_x) * (x - pond_center_x) + (y - pond_center_y) * (y - pond_center_y) - pond_radius * pond_radius) <= 0;
}

bool mountain(int x, int y) {
    return ((x - mountain_center_x) * (x - mountain_center_x) + (y - mountain_center_y) * (y - mountain_center_y) - mountain_radius * mountain_radius) <= 0;
}

bool river(int x, int y) {
    return ((x - pond_center_x) * (x - pond_center_x) + (y - pond_center_y) * (y - pond_center_y) - pond_radius * pond_radius) <= 0;
}

// Function to render the 3D terrain
void renderTerrain(int width, int height) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBegin(GL_TRIANGLES);
    for (int x = 0; x < width - 1; ++x) {
        for (int y = 0; y < height - 1; ++y) {

            double perlinValues[4];

            if (pond(x,y)) {

                perlinValues[0] = 0;//perlin(x, y, 0, pondNoiseLimit, pondOctaves, pondPersistence, pondLacunarity, pondInterpolation);
                perlinValues[1] = 0;//perlin(x + 1, y, 0, pondNoiseLimit, pondOctaves, pondPersistence, pondLacunarity, pondInterpolation);
                perlinValues[2] = 0;//perlin(x, y + 1, 0, pondNoiseLimit, pondOctaves, pondPersistence, pondLacunarity, pondInterpolation);
                perlinValues[3] = 0;//perlin(x + 1, y + 1, 0, pondNoiseLimit, pondOctaves, pondPersistence, pondLacunarity, pondInterpolation);

            } else if (mountain(x,y)) {

                perlinValues[0] = perlin(x, y, 0, mountainNoiseLimit, mountainOctaves, mountainPersistence, mountainLacunarity, mountainInterpolation);
                perlinValues[1] = perlin(x + 1, y, 0, mountainNoiseLimit, mountainOctaves, mountainPersistence, mountainLacunarity, mountainInterpolation);
                perlinValues[2] = perlin(x, y + 1, 0, mountainNoiseLimit, mountainOctaves, mountainPersistence, mountainLacunarity, mountainInterpolation);
                perlinValues[3] = perlin(x + 1, y + 1, 0, mountainNoiseLimit, mountainOctaves, mountainPersistence, mountainLacunarity, mountainInterpolation);

            } else if (river(x,y)) {

                perlinValues[0] = perlin(x, y, 0, riverNoiseLimit, riverOctaves, riverPersistence, riverLacunarity, riverInterpolation);
                perlinValues[1] = perlin(x + 1, y, 0, riverNoiseLimit, riverOctaves, riverPersistence, riverLacunarity, riverInterpolation);
                perlinValues[2] = perlin(x, y + 1, 0, riverNoiseLimit, riverOctaves, riverPersistence, riverLacunarity, riverInterpolation);
                perlinValues[3] = perlin(x + 1, y + 1, 0, riverNoiseLimit, riverOctaves, riverPersistence, riverLacunarity, riverInterpolation);

            } else {

                perlinValues[0] = perlin(x, y, 0, defaultNoiseLimit, defaultOctaves, defaultPersistence, defaultLacunarity, defaultInterpolation);
                perlinValues[1] = perlin(x + 1, y, 0, defaultNoiseLimit, defaultOctaves, defaultPersistence, defaultLacunarity, defaultInterpolation);
                perlinValues[2] = perlin(x, y + 1, 0, defaultNoiseLimit, defaultOctaves, defaultPersistence, defaultLacunarity, defaultInterpolation);
                perlinValues[3] = perlin(x + 1, y + 1, 0, defaultNoiseLimit, defaultOctaves, defaultPersistence, defaultLacunarity, defaultInterpolation);

            }

            vector<double>colors;
            // Draw the first triangle
            setColor(perlinValues[0]);
            glVertex3d(x, y, perlinValues[0]);

            setColor(perlinValues[1]);
            glVertex3d(x + 1, y, perlinValues[1]);

            setColor(perlinValues[2]);
            glVertex3d(x, y + 1, perlinValues[2]);

            // Draw the second triangle
            setColor(perlinValues[0]);
            glVertex3d(x + 1, y, perlinValues[1]);

            setColor(perlinValues[1]);
            glVertex3d(x + 1, y + 1, perlinValues[3]);

            setColor(perlinValues[2]);
            glVertex3d(x, y + 1, perlinValues[2]);

        }
    }
    glEnd();
    glutSwapBuffers();
}

// Function to initialize OpenGL settings
void init() {
    glClearColor(0.2, 0.7, 1.0, 1.0);
    glEnable(GL_DEPTH_TEST);
    gluPerspective(45.0, (double)width / (double)height, 1.0, 500.0);
    gluLookAt(eye_x, eye_y, eye_z, 
    look_x, look_y, look_z, 
    up_x, up_y, up_z);
}

void SpecialInput (int key, int xMouse, int yMouse) {
    switch (key) {
        case GLUT_KEY_RIGHT:
            // slide_u(2.0f);
            glTranslatef(0, -2, 0);
            glutPostRedisplay();
            break;
        case GLUT_KEY_LEFT:
            // slide_u(-2.0f);
            glTranslatef(0, 2, 0);
            glutPostRedisplay();
            break;
        case GLUT_KEY_UP:
            // slide_v(0.05f);
            glTranslatef(0, 0, -2);
            glutPostRedisplay();
            break;
        case GLUT_KEY_DOWN:
            // slide_v(-0.05f);
            glTranslatef(0, 0, 2);
            glutPostRedisplay();
            break;
        default:
            break;
    }
}

void keyInput (unsigned char key, int xMouse, int yMouse) {
    switch (key) {
        case '+':
            // zoom(0.05f);
            glTranslatef(2, 0, 0);
            glutPostRedisplay();
            break;
        case '-':
            // zoom(-0.05f);
            glTranslatef(-2, 0, 0);
            glutPostRedisplay();
            break;
        case 'y':
            // yaw(5);
            glRotatef(0.5, 0, 0, 1);
            glutPostRedisplay();
            break;
        case 'Y':
            // yaw(-5);
            glRotatef(-0.5, 0, 0, 1);
            glutPostRedisplay();
            break;
        case ' ':
            reset();
            break;
        default:
            break;
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1920, 1080);
    glutCreateWindow("3D Terrain Generation");

    init();

    glutDisplayFunc([]() { renderTerrain(width, height); });

    glutKeyboardFunc(keyInput);
    glutSpecialFunc(SpecialInput);

    glutMainLoop();

    return 0;
}
