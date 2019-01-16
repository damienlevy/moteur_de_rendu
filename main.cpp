#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "cmath"
#include <cstdlib>
#include "tgaimage.h"


const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);



std::string split_perso(std::string mot){
  std::string tmp="";
  
  int i = 0;
  while(mot[i] != '/'){
    
    tmp += mot[i];
    //std::cout << tmp <<std::endl;
    i++;
  }
  //std::cout << tmp <<std::endl;
  return tmp;
}


void read(std::string name, 
          std::vector< std::vector<float> > &coordonne, 
          std::vector< std::vector<int> > &point){
  //std::vector< std::vector<float> > coordonne;

    std:: ifstream fichier(name.c_str());
    if(fichier){
    std:: string ligne; 
    char c;
    //Tant qu'on n'est pas à la fin, on lit
    while(fichier >> ligne) { //permet de lire mot a mot
      while(ligne == "v"){
          std::vector<float> point;
          fichier >> ligne;
          point.push_back( atof(ligne.c_str()));
          //	std::cout << ligne <<std::endl;
          fichier >> ligne;
          point.push_back( atof(ligne.c_str()));
          //	std::cout << ligne <<std::endl;
          fichier >> ligne;
          fichier >> ligne;
          coordonne.push_back(point);
      }
      while(ligne == "f"){
        
        for(int i = 0 ; i < 3 ; i++){
          fichier >> ligne;
          std::string s = split_perso(ligne);
          std::cout << s <<std::endl;
         
        }
         fichier >> ligne;

        
      }
  
	  
    }
  }

  else{
    std:: cout << "ERREUR: Impossible d'ouvrir le fichier en lecture." << std::endl;
  }

}




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
  TGAImage image(800, 800, TGAImage::RGB);
  
  std::vector< std::vector<float> > coordonne;
  std::vector< std::vector<int> > pnt;
  read("african_head.obj",coordonne,pnt);
  std::vector<float> point;
  int size = coordonne.size(); 
  for(int i = 0 ; i < size; i++){
    point = coordonne[i];
    // std::cout <<point[0] << std::endl;
   // std::cout << ligne <<std::endl;

    image.set((point[0]+1)*800/2,(1+point[1])*800/2,white);
    }
  //image.set(-0.000581696,-0.734665 , red);
  //  line(13, 20, 80, 40, image, white); 
  // line(20, 13, 40, 80, image, red); 
  // line(80, 40, 13, 20, image, red);
  
  image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
  image.write_tga_file("output.tga");
  return 0;
}
