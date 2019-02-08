#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h> 
#include <time.h>
#include "cmath"
#include <cstdlib>
#include "tgaimage.h"
#include "point.h"


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
          point.push_back( atof(ligne.c_str()));
          fichier >> ligne;
          coordonne.push_back(point);
      }
      while(ligne == "f"){
        std::vector<int> coord; 
        for(int i = 0 ; i < 3 ; i++){
          fichier >> ligne;
          std::string s = split_perso(ligne);
          coord.push_back(atof(s.c_str())-1);  
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

Point3DF barycentre(Point3DF p1,Point3DF p2,Point3DF p3,Point3DF p){
  Point3DF temp1(p3.getX()-p1.getX(),p2.getX()-p1.getX(),p1.getX()-p.getX());
  Point3DF temp2(p3.getY()-p1.getY(),p2.getY()-p1.getY(),p1.getY()-p.getY());

  Point3DF bary = temp1^(temp2);
  float comp = std::abs(bary.getZ());
  if(comp>1e-2){
    
   return Point3DF(1.f-(bary.getX()+bary.getY())/bary.getZ(),
                  bary.getY()/bary.getZ(),
                  bary.getX()/bary.getZ());
     
  }
  return Point3DF(-1,1,1);
}

void triangle(Point3DF *p,TGAImage &image, TGAColor color){
  float *zbuffer = new float[width*height];
  Point2DF clamp(image.get_width()-1 , image.get_height()-1);
  float xMin(image.get_width()-1);
  float yMin(image.get_height()-1);
  float xMax(0),yMax(0);
  for(int i = 0 ; i < 3 ; i++){
    float tmp1 = p[i].getX();
    float tmp2 = p[i].getY();
    tmp1 = std::min( xMin, tmp1);
    xMin = std::max(0.f ,tmp1);

    yMin = std::max(0.f,std::min(yMin,tmp2));
    xMax = std::min(clamp.getX(),std::max(xMax,tmp1));
    yMax = std::min(clamp.getX(),std::max(yMax,tmp2));
    
  }

  Point3DF temp;
  float z(0);
  for(int i=xMin; i<= xMax ; i++){
    for(int j = yMin ; j <= yMax ; j++){
      temp = Point3DF(i,j,z);
      Point3DF bary = barycentre(p[0],p[1],p[2],temp);
      if(bary.getX()<0 || bary.getY() <0 || bary.getZ()<0) continue;
      
      z = p[0].getZ()*bary.getX()+p[1].getZ()*bary.getY()+p[2].getZ()*bary.getZ();
      if(zbuffer[int(i+j*width)] < z){
        //std::cout<<"coucou"<<std::endl;
        zbuffer[int(i+j*width)] = z;
        image.set(i,j,color);

      }
      
    }
  }
}


void triangle(Point3DF p1,Point3DF p2,Point3DF p3,TGAImage &image, TGAColor color){
  float *zbuffer = new float[width*height];
  int max_X = std::max(p1.getX(),std::max(p2.getX(),p3.getX()));
  int max_Y = std::max(p1.getY(),std::max(p2.getY(),p3.getY()));
  int min_X = std::min(p1.getX(),std::min(p2.getX(),p3.getX()));
  int min_Y = std::min(p1.getY(),std::min(p2.getY(),p3.getY()));
  Point2DF vector1(p2.getX()-p1.getX(),p2.getY()-p1.getY());
  Point2DF vector2(p3.getX()-p1.getX(),p3.getY()-p1.getY());
  float s,t;
  Point3DF temp;
  float z(0);
  for(int x = min_X ; x <= max_X ; x++){
    for(int y = min_Y; y<= max_Y ;y++){
      temp = Point3DF(x,y,z);
      Point3DF bary = barycentre(p1,p2,p3,temp);
      Point2DF q(x-p1.getX(),y-p1.getY());
      s = q.crossProduct(vector2) / vector1.crossProduct(vector2);
      t = vector1.crossProduct(q) / vector1.crossProduct(vector2);

      if((s >= 0) && (t >= 0) && (s + t <= 1)){
        z = 0;

        image.set(x,y,color);
      }

    }
  }
}

void triangle2(Point2DF p1,Point2DF p2,Point2DF p3,TGAImage &image, TGAColor color){
  float A,B;
  if(p1.getY()>p2.getY()){ 
    std::swap(p1,p2);


  }
  if(p1.getY()>p3.getY()){
    std::swap(p1,p3);
    
  }
  if(p2.getY()>p3.getY()){
    std::swap(p2,p3);  
  }
  int hauteur = p3.getY() - p1.getY();

  for(int i = 0 ; i < hauteur ; i++){
    
    bool second_moitier = i > p2.getY()-p1.getY() || p2.getY()== p1.getY();
    int hauteur_segment;
    if(second_moitier){
      hauteur_segment = p3.getY()-p2.getY();
        
    }else{
      hauteur_segment = p2.getY()-p1.getY();
       
    }
    float alpha = (float) i/hauteur;
    float beta; 
    if(second_moitier){
      beta = (float) (i - (p2.getY()-p1.getY()) )/hauteur_segment;
    }else{
      beta = (float) i/hauteur_segment;

    }

      A = p1.getX() + ((p3.getX()-p1.getX()) * alpha);
      
      if(second_moitier){
        B = p2.getX() + (p3.getX()-p2.getX())*beta;
      }else{
        B = p1.getX() + ((p2.getX() - p1.getX()) * beta);
      }
      
      if(A>B) std::swap(A,B);
      int x = (int) A;
      int y = (int) B;
      
      for(int j = x; j <=y ; j++){
       
        image.set(j,p1.getY()+i,color); 

      }
       
    }

}

int main() {//int argc, char** argv


/*///////test////////
  Point3DI a(2,5,3);
  Point3DI b(1,1,1);
 

  if(a != b){
    //Point3DI pp(a.getY()*b.getZ()-a.getZ()*b.getY() , a.getZ()*b.getX()-a.getX()*b.getZ() , a.getX()*b.getY()-a.getY()*b.getX());
    Point3DI pp = a^b;
    std::cout<< pp <<std::endl;
  }

///////////////*/
  srand (time(NULL)); //pour le random

  TGAImage image(width, height, TGAImage::RGB);
  Point3DF lumiere(0,0,-1);
  float intensite(0);
  
  std::vector< std::vector<float> > coordonne;
  std::vector< std::vector<int> > pnt;
  
  read("african_head.obj",coordonne,pnt); //v , f
  std::vector<int> p; //pour recuperer les 3 point à relier
  int size_pnt = pnt.size();

  //boucle de dessin des triangles
  Point3DF world_coords[3];
  Point3DF n;

  for(int i = 0 ; i < size_pnt ; i++ ){

    p = pnt[i]; //pnt : f 
    std::vector<float> p1;
    std::vector<float> p2;
    std::vector<float> p3;
 
    p1 = coordonne[p[0]]; //coordonne : v
    p2 = coordonne[p[1]];
    p3 = coordonne[p[2]];

    world_coords[0] = Point3DF((p1[0]+ 1) * width/2,(p1[1]+ 1) * height/2,p1[2]);
    world_coords[1] = Point3DF((p2[0]+ 1) * width/2,(p2[1]+ 1) * height/2,p2[2]);
    world_coords[2] = Point3DF((p3[0]+ 1) * width/2,(p3[1]+ 1) * height/2,p3[2]);

    Point2DF point1((p1[0]+ 1) * width/2,(p1[1]+ 1) * height/2);
    Point2DF point2((p2[0]+ 1) * width/2,(p2[1]+ 1) * height/2);
    Point2DF point3((p3[0]+ 1) * width/2,(p3[1]+ 1) * height/2);

    n = (world_coords[2]-world_coords[0])^(world_coords[1]-world_coords[0]);
    n.normalize();
    intensite = n*lumiere;

    if(intensite>0){
      Point3DF pp[3];
      for(int i = 0 ; i < 3 ;i++){
        pp[i] = world_coords[i];
      }
      
      triangle(world_coords[0],world_coords[1],world_coords[2],image,TGAColor(intensite*255,intensite*255,intensite*255,255));
      //triangle(point1,point2,point3,image,TGAColor(intensite*255,intensite*255,intensite*255,255));
      //triangle(pp,image,TGAColor(intensite*255,intensite*255,intensite*255,255));
    }
    
   
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
