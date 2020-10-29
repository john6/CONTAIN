#include <CONTAIN/Material.h>



Material::Material() :
	dens {0.0f}, rest {0.0f},
	statFrict {0.0f}, dynaFrict {0.0f}
{
}

Material::Material(float i_dens, float i_rest, float i_statFrict, float i_dynaFrict) :
	dens{ i_dens }, rest{ i_rest }, statFrict{ i_statFrict }, dynaFrict{ i_dynaFrict}
{
}


Material::~Material()
{
}
