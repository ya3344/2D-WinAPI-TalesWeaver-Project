#pragma once
class Obj;
class Image;
class MyBmp;

typedef list<Obj*> OBJ_LIST;
typedef list<Obj*>::iterator OBJ_LIST_POS;

typedef vector<Obj*> OBJ_VECTOR;
typedef vector<Obj*>::iterator OBJ_VECTOR_POS;

typedef list<Image*> IMAGE_LIST;

typedef pair<wstring, MyBmp*> bmpMapPair;
typedef pair<wstring, Image*> EffectMapPair;