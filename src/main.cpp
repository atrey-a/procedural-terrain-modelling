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

                perlinValues[0] = perlin(x, y, 0, pondNoiseLimit, pondOctaves, pondPersistence, pondLacunarity, pondInterpolation);
                perlinValues[1] = perlin(x + 1, y, 0, pondNoiseLimit, pondOctaves, pondPersistence, pondLacunarity, pondInterpolation);
                perlinValues[2] = perlin(x, y + 1, 0, pondNoiseLimit, pondOctaves, pondPersistence, pondLacunarity, pondInterpolation);
                perlinValues[3] = perlin(x + 1, y + 1, 0, pondNoiseLimit, pondOctaves, pondPersistence, pondLacunarity, pondInterpolation);

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

                perlinValues[0] = perlin(x, y, 0, riverNoiseLimit, riverOctaves, riverPersistence, riverLacunarity, riverInterpolation);
                perlinValues[1] = perlin(x + 1, y, 0, riverNoiseLimit, riverOctaves, riverPersistence, riverLacunarity, riverInterpolation);
                perlinValues[2] = perlin(x, y + 1, 0, riverNoiseLimit, riverOctaves, riverPersistence, riverLacunarity, riverInterpolation);
                perlinValues[3] = perlin(x + 1, y + 1, 0, riverNoiseLimit, riverOctaves, riverPersistence, riverLacunarity, riverInterpolation);

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
    gluLookAt(550.0, 250.0, 100.0, 
    0.0, 250.0, -200.0, 
    0.0, 0.0, 50.0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1920, 1080);
    glutCreateWindow("3D Terrain Generation");

    init();

    glutDisplayFunc([]() { renderTerrain(width, height); });

    glutMainLoop();

    return 0;
}
