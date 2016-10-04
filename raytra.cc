#include "raytra.h"

Camera::Camera (Point &pos, Vector3 &dir,
               float d, float iw, float ih,
               int pw, int ph)
    : _eye(pos), _d(d), _iw(iw), _ih(ih), _pw(pw), _ph(ph) 
{
    
}



