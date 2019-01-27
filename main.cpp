#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h> 
#include <time.h>
#include "cmath"
#include <cstdlib>
#include "tgaimage.h"



const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor green = TGAColor(0, 255, 0, 255);

int const width = 800;
int const height = 800;

std::string split_perso(std::string mot){
  std::string tmp="";
  int i = 0;
  while(mot[i] != '/'){
    
    tmp += mot[i];
    i++;
  }
  return tmp;
}


void read(std::string name, 
          std::vector< std::vector<float> > &coordonne, 
          std::vector< std::vector<int> > &pnt){
  //std::vector< std::vector<float> > coordonne;

    std:: ifstream fichier(name.c_str());
    if(fichier){
    std:: string ligne;
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
        std::vector<int> coord; 
        for(int i = 0 ; i < 3 ; i++){
          fichier >> ligne;
          std::string s = split_perso(ligne);
          coord.push_back(atof(s.c_str())-1);
          //std::cout << s <<std::endl;
         
        }
        pnt.push_back(coord);
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

TGAColor rand_color(){
  TGAColor color;
  
  int rnd1 = rand() % 256;
  int rn2  = rand() % 256;
  int rnd3 = rand() % 256;
  color = TGAColor(rnd1, rn2, rnd3, 255);
  return color;
}


void triangle(std::vector<float> p1,
              std::vector<float> p2,
              std::vector<float> p3,
              TGAImage &image, 
              TGAColor color){

    float A,B;

    float p1x = (p1[0]+ 1) * width/2;
    float p2x = (p2[0]+ 1) * width/2;
    float p3x = (p3[0]+ 1) * width/2;

    float p1y = (p1[1]+ 1) * height/2;
    float p2y = (p2[1]+ 1) * height/2;
    float p3y = (p3[1]+ 1) * height/2;

   
    if(p1y>p2y){ 
      std::swap(p1y,p2y);
      std::swap(p1x,p2x);

      }
    if(p1y>p3y){
      std::swap(p1y,p3y);
      std::swap(p1x,p3x);
      }
    if(p2y>p3y){
      std::swap(p2x,p3x);
      std::swap(p2y,p3y);  
    }
    
    int hauteur = p3y - p1y;

    for(int i = 0 ; i < hauteur ; i++){
    
    
      bool second_moitier = i > p2y-p1y || p2y == p1y;
      
      int hauteur_segment;
      if(second_moitier){
        hauteur_segment = p3y-p2y;
        
      }else{
        hauteur_segment = p2y-p1y;
       
      }
      float alpha = (float) i/hauteur;
      float beta; 
      if(second_moitier){
        beta = (float) (i - (p2y-p1y) )/hauteur_segment;
      }else{
        beta = (float) i/hauteur_segment;

      }

      A = p1x + ((p3x-p1x) * alpha);
      
      if(second_moitier){
        B = p2x + (p3x-p2x)*beta;
      }else{
        B = p1x + ((p2x - p1x) * beta);
      }
      
      if(A>B) std::swap(A,B);
      int x = (int) A;
      int y = (int) B;
      
      for(int j = x; j <=y ; j++){
       
        image.set(j,p1y+i,color); 

      }
       
    }

}


int main() {//int argc, char** argv

  srand (time(NULL)); //pour le random

  TGAImage image(width, height, TGAImage::RGB);
  
  std::vector< std::vector<float> > coordonne;
  std::vector< std::vector<int> > pnt;
  
  read("african_head.obj",coordonne,pnt);
  std::vector<int> p; //pour recuperer les 3 point à relier
  int size_pnt = pnt.size();

  //boucle de dessin des triangles
  for(int i = 0 ; i < size_pnt ; i++ ){

    p = pnt[i];
    std::vector<float> p1;
    std::vector<float> p2;
    std::vector<float> p3;
 
    p1 = coordonne[p[0]];
    p2 = coordonne[p[1]];
    p3 = coordonne[p[2]];
   
   
    triangle(p1,p2,p3,image,rand_color());
   
  }


/*
  std::vector<float> p1;
  p1.push_back(0.40);
  p1.push_back(0.80);
  std::vector<float> p2;
  p2.push_back(0.80);
  p2.push_back(0.20);
  std::vector<float> p3;
  p3.push_back(0.20);
  p3.push_back(0.13);
  triangle(p1,p2,p3,image,rand_color());
*/
  image.flip_vertically(); 
  image.write_tga_file("output.tga");
  return 0;
}
