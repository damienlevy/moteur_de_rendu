#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "cmath"
#include <cstdlib>
#include "tgaimage.h"




std::vector< std::vector<float> > read(std::string name){
  std::vector< std::vector<float> > coordonne;
  std::vector<float> point;
  std:: ifstream fichier(name.c_str());
  if(fichier){
    std:: string ligne; 
     
    //Tant qu'on n'est pas à la fin, on lit
    while(fichier >> ligne) {
      while(ligne == "v"){
	fichier >> ligne;
	point.push_back( atof(ligne.c_str()));
	fichier >> ligne;
	point.push_back( atof(ligne.c_str()));
	fichier >> ligne;
	fichier >> ligne;
	coordonne.push_back(point);
      }
     


      std::cout << ligne <<std::endl;
	  
	  
    }
  }

  else{
    std:: cout << "ERREUR: Impossible d'ouvrir le fichier en lecture." << std::endl;
  }
  return coordonne;
}

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);

void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) { 
  bool steep = false; 
  if (std::abs(x0-x1)<std::abs(y0-y1)) { 
    std::swap(x0, y0); 
    std::swap(x1, y1); 
    steep = true; 
  } 
  if (x0>x1) { 
    std::swap(x0, x1); 
    std::swap(y0, y1); 
  } 
  int dx = x1-x0; 
  int dy = y1-y0; 
  int derror2 = std::abs(dy)*2; 
  int error2 = 0; 
  int y = y0; 
  for (int x=x0; x<=x1; x++) { 
    if (steep) { 
      image.set(y, x, color); 
    } else { 
      image.set(x, y, color); 
    } 
    error2 += derror2; 
    if (error2 > dx) { 
      y += (y1>y0?1:-1); 
      error2 -= dx*2; 
    } 
  } 
} 




int main(int argc, char** argv) {
  TGAImage image(100, 100, TGAImage::RGB);
 
  std::vector< std::vector<float> > coordonne = read("african_head.obj");

  image.set(-0.000581696,-0.734665 , red);
  line(13, 20, 80, 40, image, white); 
  line(20, 13, 40, 80, image, red); 
  line(80, 40, 13, 20, image, red);

  image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
  image.write_tga_file("output.tga");
  return 0;
}
