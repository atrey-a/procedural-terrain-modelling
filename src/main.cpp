#include <GL/glut.h>
#include <cmath>
#include <vector>
using namespace std;

const int width = 500;
const int height = 500;
const int noiseLimit = 1;

int octaves = 6;
double persistence = 1;
double lacunarity = 0.3;

// Function to generate Perlin noise in 3D
double noise(int x, int y, int z) {
    int n = x + y * 57 + z * 131;
    n = (n << 20) ^ n;
    return (1.0 - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0)*noiseLimit;
}

// Function to interpolate between two values
double interpolate(double a, double b, double t) {
    // double ft = t * 3.1415927;
    // double f = (1.0 - cos(ft)) * 0.5;
    // return a * (1.0 - f) + b * f;
    return a * (1.0 - t) + b * t;
}

// Function to generate 3D Perlin noise
double perlinCalculate(int x, int y, int z) {
    // x /= 0.2;
    // y /= 0.2;
    // z /= 2;
    double n = noise(x, y, z);
    double i1 = interpolate(noise(x - 1, y, z), noise(x + 1, y, z), n);
    double i2 = interpolate(noise(x, y - 1, z), noise(x, y + 1, z), n);
    double i3 = interpolate(noise(x, y, z - 1), noise(x, y, z + 1), n);
    return interpolate(interpolate(i1, i2, n), i3, n);
}

double perlin(int x, int y, int z){
    double total = 0.0;
    double frequency = 2;
    double amplitude = 1;

    for (int octave = 0; octave < octaves; ++octave) {
        total += amplitude * perlinCalculate(x * frequency, y * frequency, z * frequency);
        frequency *= lacunarity;
        amplitude *= persistence;
    }

    // Normalize the result to the range [0, 1]
    return total;
}

void setColor(double perlinValue) {
    double r, g, b;
    if (perlinValue > noiseLimit*10*0.75) { // dark brown
        r = 0.341;
        g = 0.224;
        b = 0.051;
    }
    else if(perlinValue > noiseLimit*8*0.6){  // light brown
        r = 0.702;
        g = 0.459;
        b = 0.114;
    }
    else if(perlinValue > noiseLimit*2*0.4){  // dark green
        r = 0.251;
        g = 0.439;
        b = 0.059;
    }
    else if(perlinValue > noiseLimit*0.2){  // light green
        r = 0.455;
        g = 0.812;
        b = 0.082;
    }
    else if(perlinValue > noiseLimit*0.01){  // even lighter green
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

// Function to render the 3D terrain
void renderTerrain(int width, int height) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBegin(GL_TRIANGLES);
    for (int x = 0; x < width - 1; ++x) {
        for (int y = 0; y < height - 1; ++y) {

            double perlinValues[4];
            perlinValues[0] = perlin(x, y, 0);
            perlinValues[1] = perlin(x + 1, y, 0);
            perlinValues[2] = perlin(x, y + 1, 0);
            perlinValues[3] = perlin(x + 1, y + 1, 0);

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