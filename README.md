# Perlin Noise Image Generator

## Technology
This project uses the C language and the MinilibX graphics library for Linux to generate images using Perlin noise. Perlin noise is a type of gradient noise developed by Ken Perlin in 1983. It is widely used in computer graphics for effects such as fire, smoke and clouds, as well as to simulate natural phenomena such as the distribution of hair on an animal or the shape of a mountain range.


## How it Works
The program generates a 2D array of noise values using the Perlin Noise algorithm, then maps these values to a range of colors to create an image. The result is a smoothly varying, non-repeating image that can be used for textures, height maps for terrain generation, or any place where you need interesting random data.



## Input explanation: ./perlinDisplay <Seed> <octave> <persistence> <lacutarity>
### Seed: 42, octave 8 , persistence 1.0, lacutarity 2.0

## Example
Here's an example of an image generated with this program:

### Input ./perlinDisplay 42 8 1 2 
![Example1](screen/Noise_42_8_1_2.png)

### Input ./perlinDisplay 12345 8 1.0 2.0
![Example2](screen/Noise_12345_8_1_2.png)

### Input ./perlinDisplay 42 1 1 2 (One octave)
![Example3](screen/Noise_42_1_1_2.png)

### Input ./perlinDisplay 42 4 1 2 
![Example4](screen/Noise_42_4_1_2.png)

### Input ./perlinDisplay 42 8 2 2 
![Example5](screen/Noise_42_8_2_2.png)

