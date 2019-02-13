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
#include "matrix.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor green = TGAColor(0, 255, 0, 255);

int const width = 800;
int const height = 800;
int const depth  = 255;

std::string split_perso(std::string mot){
  std::string tmp="";
  int i = 0;
  while(mot[i] != '/'){
    tmp += mot[i];
    i++;
  }
  return tmp;
}
std::string split_perso2(std::string mot){
  std::string tmp="";
  int i = 0;
  while(mot[i] != '/'){
    i++;
  }
  i++;
  while(mot[i]!='/'){
    tmp+=mot[i];
    i++;
  }
  return tmp;
}
std::string split_perso3(std::string mot){
  std::string tmp="";
  int i = 0;
  while(mot[i] != '/'){
    i++;
  }
  i++;
  while(mot[i] != '/'){
    i++;  
  }
  i++;
  while(mot[i]!='/'){ 
    tmp+=mot[i];
    i++;
  }
  return tmp;
}

Point3DF matrixToPoint(Matrix m){
  return Point3DF(m[0][0]/m[3][0],m[1][0]/m[3][0],m[2][0]/m[3][0]);
}

Matrix pointToMatrix(Point3DF p){
  Matrix m(4,1);
  m[0][0] = p.getX();
  m[1][0] = p.getY();
  m[2][0] = p.getZ();
  m[3][0] = 1.f;
  return m;
}

Matrix viewport(int x, int y, int w, int h) {
    Matrix m = Matrix::identity(4);
    m[0][3] = x+w/2.f;
    m[1][3] = y+h/2.f;
    m[2][3] = depth/2.f;

    m[0][0] = w/2.f;
    m[1][1] = h/2.f;
    m[2][2] = depth/2.f;
    return m;
}
Matrix lookat(Point3DF eye, Point3DF center, Point3DF up) {
    Point3DF z = (eye-center).normalize();
    Point3DF x = (up^z).normalize();
    Point3DF y = (z^x).normalize();
    Matrix Minv = Matrix::identity(4);
    Matrix Tr   = Matrix::identity(4);

    Minv[0][0] = x.getX();
    Minv[0][1] = x.getY();
    Minv[0][2] = x.getZ();
    Minv[1][0] = y.getX();
    Minv[1][1] = y.getY();
    Minv[1][2] = y.getZ();
    Minv[2][0] = z.getX();
    Minv[2][1] = z.getY();
    Minv[2][2] = z.getZ();
    Tr[0][3] = -center.getX();
    Tr[1][3] = -center.getY();
    Tr[2][3] = -center.getZ();
  return Matrix(Minv*Tr);
}

void read(std::string name, 
          std::vector< std::vector<float> > &coordonne, 
          std::vector< std::vector<int> > &pnt,
          std::vector<Point3DF> &coordTexture,
          std::vector<Point3DI> &pointTexture,
          std::vector<Point3DF> &vn,
          std::vector<Point3DI> &f3
          ){
  int compt = 0;
    std:: ifstream fichier(name.c_str());
    if(fichier){
    std:: string ligne;
    float tab[3];
    float temp[3];
    //Tant qu'on n'est pas à la fin, on lit
    while(fichier >> ligne) { //permet de lire mot a mot
      while(ligne == "v"){
          std::vector<float> point;     
          fichier >> ligne;
          point.push_back( atof(ligne.c_str()));
          fichier >> ligne;
          point.push_back( atof(ligne.c_str()));
          fichier >> ligne;
          point.push_back( atof(ligne.c_str()));
          fichier >> ligne;
          coordonne.push_back(point);
      }
      while(ligne == "vt"){
        for(int i = 0 ; i<3;i++){
          fichier >> ligne;
          tab[i] = atof(ligne.c_str());
        }
        coordTexture.push_back(Point3DF(tab[0],tab[1],tab[2]));
        fichier >> ligne;
      }
      while(ligne == "vn"){
        compt++;
        for(int i = 0 ; i<3;i++){
          fichier >> ligne;
          tab[i] = atof(ligne.c_str());
        }
        vn.push_back(Point3DF(tab[0],tab[1],tab[2]));
        fichier >> ligne;
      }
      while(ligne == "f"){
        std::vector<int> coord; 
        for(int i = 0 ; i < 3 ; i++){
          fichier >> ligne;
          std::string s = split_perso(ligne);
         
          coord.push_back(atof(s.c_str())-1);  
          tab[i] = atof(split_perso2(ligne).c_str())-1;
          temp[i] = atof(split_perso3(ligne).c_str())-1;
          
        }
        pointTexture.push_back(Point3DI(tab[0],tab[1],tab[2]));
        f3.push_back(Point3DI(temp[0],temp[1],temp[2]));
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
  if(comp < 1){
    
     return Point3DF(-1,1,1);
     
  }
  return Point3DF(1.f-(bary.getX()+bary.getY())/bary.getZ(),
                  bary.getY()/bary.getZ(),
                  bary.getX()/bary.getZ());
 
}



void triangle(Point3DF p1,Point3DF p2,Point3DF p3,float *zbuffer,TGAImage &image, TGAImage &texture,float *intensit,float intensite,Point3DF *pointText){
  int max_X = std::max(p1.getX(),std::max(p2.getX(),p3.getX()));
  int max_Y = std::max(p1.getY(),std::max(p2.getY(),p3.getY()));
  int min_X = std::min(p1.getX(),std::min(p2.getX(),p3.getX()));
  int min_Y = std::min(p1.getY(),std::min(p2.getY(),p3.getY()));
  
  Point3DF temp;
  float z(0);
  float textX(0) , textY(0);
  for(float x = min_X ; x <= max_X ; x++){
    for(float y = min_Y; y<= max_Y ;y++){
      temp = Point3DF(x,y,z);
      Point3DF bary = barycentre(p1,p2,p3,temp);
      if(bary.getX()<0 || bary.getY() <0 || bary.getZ()<0) continue;
      z = 0;
      z = (p1.getZ()*bary.getX())+(p2.getZ()*bary.getY())+(p3.getZ()*bary.getZ());
      if(zbuffer[int(x+y*width)] < z ){
        zbuffer[int(x+y*width)] = z ;
        textX = pointText[0].getX()*bary.getX() + pointText[1].getX()*bary.getY() + pointText[2].getX()*bary.getZ();
        textY = pointText[0].getY()*bary.getX() + pointText[1].getY()*bary.getY() + pointText[2].getY()*bary.getZ();
        intensite = -(intensit[0]*bary.getX() + intensit[1]*bary.getY() + intensit[2]*bary.getZ());
        TGAColor color = texture.get(textX * texture.get_width() , textY * texture.get_height());
        image.set(x,y,color*intensite);
      }
    }
  }
}


int main() {//int argc, char** argv

  srand (time(NULL)); //pour le random

  TGAImage image(width, height, TGAImage::RGB);
  TGAImage texture;
  texture.read_tga_file("african_head_diffuse.tga");
  //texture.read_tga_file("african_head_spec.tga");
  //texture.read_tga_file("diablo3_pose_diffuse.tga");  
  
  texture.flip_vertically();
  Point3DF lumiere(0,0,-1);
  lumiere.normalize();
  Point3DF eye(3,1,3);
  Point3DF center(0,0,0);
  float intensit[3];
  float intensite(0);
  float *zbuffer = new float[width*height];
  Point3DF pointText[3];
  Point3DF pointNorm[3];
  Point3DI temp;
  Point3DI temp2;
  for (int i=width*height; i--; zbuffer[i] = -1);
  
  std::vector< std::vector<float> > coordonne;
  std::vector< std::vector<int> > pnt;
  std::vector<Point3DF>coordText;
  std::vector<Point3DI> pointTexture;
  std::vector<Point3DF> vn;
  std::vector<Point3DI> f3;
  read("african_head.obj",coordonne,pnt,coordText,pointTexture , vn,f3); //v , f , vt ,f/ / , vn
//  read("diablo3_pose.obj",coordonne,pnt,coordText,pointTexture , vn,f3); //v , f , vt ,f/ / , vn
  
  std::vector<int> p; //pour recuperer les 3 point à relier
  int size_pnt = pnt.size();

  Matrix vue = lookat(eye,center,Point3DF(0,1,0));
  Matrix Projection = Matrix::identity(4);
  Matrix ViewPort   = viewport(width/8, height/8, width*3/4, height*3/4);
  Projection[3][2] = -1.f/(eye-center).norm();
  Matrix z = (ViewPort*Projection*vue);

  Point3DF world_coords[3];
  Point3DF screen[3];
  Point3DF n;
//boucle de dessin des triangles
  for(int i = 0 ; i < size_pnt ; i++ ){
    temp = pointTexture[i];
    temp2 = f3[i];
    pointText[0] = coordText[temp.getX()];
    pointText[1] = coordText[temp.getY()];
    pointText[2] = coordText[temp.getZ()];
    p = pnt[i]; //pnt : f 
    std::vector<float> p1;
    std::vector<float> p2;
    std::vector<float> p3;

    p1 = coordonne[p[0]]; //coordonne : v
    p2 = coordonne[p[1]];
    p3 = coordonne[p[2]];

    pointNorm[0] = vn[temp2.getX()].normalize();
    pointNorm[1] = vn[temp2.getY()].normalize();
    pointNorm[2] = vn[temp2.getZ()].normalize();
    
    intensit[0] = lumiere * pointNorm[0]; 
    intensit[1] = lumiere * pointNorm[1];
    intensit[2] = lumiere * pointNorm[2];

    world_coords[0] = Point3DF(p1[0],p1[1],p1[2]);
    world_coords[1] = Point3DF(p2[0],p2[1],p2[2]);
    world_coords[2] = Point3DF(p3[0],p3[1],p3[2]);
    for(int j = 0 ; j < 3 ; j++){
      screen[j] = matrixToPoint(ViewPort * Projection * vue * pointToMatrix(world_coords[j]));  
    }

    world_coords[0] = Point3DF((p1[0]+ 1) * width/2,(p1[1]+ 1) * height/2,p1[2]);
    world_coords[1] = Point3DF((p2[0]+ 1) * width/2,(p2[1]+ 1) * height/2,p2[2]);
    world_coords[2] = Point3DF((p3[0]+ 1) * width/2,(p3[1]+ 1) * height/2,p3[2]);

    triangle(screen[0],screen[1],screen[2],zbuffer,image,texture,intensit,intensite,pointText);

  }


  image.flip_vertically(); 
  //image.write_tga_file("output.tga");
  //image.write_tga_file("diablo.tga");
  image.write_tga_file("african_.tga");





  delete []zbuffer;
  return 0;
}
