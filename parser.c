#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "matrix.h"
#include "parser.h"


/*======== void parse_file () ==========
Inputs:   char * filename 
          struct matrix * transform, 
          struct matrix * pm,
          screen s
Returns: 
Goes through the file named filename and performs all of the actions listed in that file.
The file follows the following format:
     Every command is a single character that takes up a line
     Any command that requires arguments must have those arguments in the second line.
     The commands are as follows:
         line: add a line to the edge matrix - 
	     takes 6 arguemnts (x0, y0, z0, x1, y1, z1)
	      circle: add a circle to the edge matrix - 
	          takes 3 arguments (cx, cy, r)
		   hermite: add a hermite curve to the edge matrix -
		       takes 8 arguments (x0, y0, x1, y1, x2, y2, x3, y3)
		        bezier: add a bezier curve to the edge matrix -
			    takes 8 arguments (x0, y0, x1, y1, x2, y2, x3, y3)
			     ident: set the transform matrix to the identity matrix - 
			      scale: create a scale matrix, 
			          then multiply the transform matrix by the scale matrix - 
				      takes 3 arguments (sx, sy, sz)
				       translate: create a translation matrix, 
				           then multiply the transform matrix by the translation matrix - 
					       takes 3 arguments (tx, ty, tz)
					        xrotate: create an x-axis rotation matrix,
						    then multiply the transform matrix by the rotation matrix -
						        takes 1 argument (theta)
							 yrotate: create an y-axis rotation matrix,
							     then multiply the transform matrix by the rotation matrix -
							         takes 1 argument (theta)
								  zrotate: create an z-axis rotation matrix,
								      then multiply the transform matrix by the rotation matrix -
								          takes 1 argument (theta)
									   apply: apply the current transformation matrix to the 
									       edge matrix
									        display: draw the lines of the edge matrix to the screen
										    display the screen
										     save: draw the lines of the edge matrix to the screen
										         save the screen to a file -
											     takes 1 argument (file name)
											      quit: end parsing
See the file script for an example of the file format
IMPORTANT MATH NOTE:
the trig functions int math.h use radian mesure, but us normal
humans use degrees, so the file will contain degrees for rotations,
be sure to conver those degrees to radians (M_PI is the constant
for PI)
====================*/
void parse_file ( char * filename, 
                  struct matrix * transform, 
                  struct matrix * pm,
                  screen s) {

  FILE *f;
  char line[256];
  
  double x0 = 0;
  double x1 = 0;
  double y0 = 0;
  double y1 = 0;
  double z0 = 0;
  double z1 = 0;

  color c;
  c.red = 0;
  c.green = MAX_COLOR;
  c.blue = 0;
  
  clear_screen(s);

  if ( strcmp(filename, "stdin") == 0 ) 
    f = stdin;
  else
    f = fopen(filename, "r");
  
  while ( fgets(line, 255, f) != NULL ) {
    line[strlen(line)-1]='\0';
    printf(":%s:\n",line);  
  
    if(!strcmp(line, "line")){
      sscanf(fgets(line, 255, f), "%lf %lf %lf %lf %lf %lf", &x0, &y0, & z0, &x1, &y1, &z1);
      add_edge(pm, x0, y0, z0, x1, y1, z1);
    }
    else if(!strcmp(line, "circle")){
      sscanf(fgets(line, 255, f), "%lf %lf %lf", &x0, &y0, &z0);
      add_circle(pm, x0, y0, z0, 0.05);
    }
    else if(!strcmp(line, "ident")){
      ident(transform);
    }
    else if(!strcmp(line, "scale")){
      sscanf(fgets(line, 255, f), "%lf %lf %lf", &x0, &y0, &z0);
      matrix_mult(make_scale(x0, y0, z0), transform);
    }
    else if(!strcmp(line, "apply")){
      matrix_mult(transform, pm);
    }
    else if(!strcmp(line, "display")){
      draw_lines(pm, s, c);
      //save_extension(s, "parser.png"); // because display doesnt work for me
      display(s);
    }
    else if(!strcmp(line, "translate")){
      sscanf(fgets(line, 255, f), "%lf %lf %lf", &x0, &y0, &z0);
      matrix_mult(make_translate(x0, y0, z0), transform);
    }
    else if(!strcmp(line, "quit")){
      exit(0);
    }
    else {
      printf("I'm sorry that is not a command\n");
    }
  }
}
