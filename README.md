# Air-Friction
A simple C++ code using OpenGL to visualize the trajectory of a projectile subject to air resistance

## Physics Background

When an object moves at high velocity, air excerts a ![drag force](https://en.wikipedia.org/wiki/Drag_equation) proportional to the square of its velocity 
$$\vec F_{\rm drag}= -\gamma v^2\hat v= -\gamma \|\|v\|\|\vec v$$
which, under the force of gravity, results in the Newton equation of motion
$$\vec F=m\ddot r = -g \hat y -\gamma \|\|{\dot{\vec r}}\|\|{\dot{\vec r}}.$$

## Numerical solution

Using Runge Kutta of 4th order, we solve the equations of motion, and plot the trajectory in 3D space using OpenGL.


## Examples
![ex1](https://github.com/fgonzcat/Air-Friction/blob/main/examples/ex1_Mar-2-2024.gif)
![ex2](https://github.com/fgonzcat/Air-Friction/blob/main/examples/ex1_Mar-2-2024.png)
![ex3](https://github.com/fgonzcat/Air-Friction/blob/main/examples/3.png)

