#include "RigidBody.h"

RigidBody::RigidBody(std::shared_ptr<Shape> i_shape, Material i_material) :
	shape{ i_shape }, mat{ i_material }, vel{Vector2f(0.0f ,0.0f)},
	force{Vector2f(0.0f ,0.0f)}, angVel{0.0f}, torq{0.0f}
{
	ignoreForcesThisStep = false;
	SetMassData();
	objVerts = shape->GetPoints();
	numVerts = objVerts.size(); //TODO
	for (int i = 0; i < numVerts; i++) {
		worldVerts.push_back(Vector2f(0.0f, 0.0f));
		faceNorms.push_back(Vector2f(0.0f, 0.0f));
	}
	UpdateVertsAndNorms();
}

RigidBody::~RigidBody()
{
}

//RigidBody::RigidBody(const RigidBody & i_rb)
//{
//	ignoreForcesThisStep = false;
//	shape = i_rb.shape;
//	mat = i_rb.mat;
//	transform = i_rb.transform;
//	massD = i_rb.massD;
//	vel = i_rb.vel;
//	force = i_rb.force;
//	angVel = i_rb.angVel;
//	torq = i_rb.torq;
//	SetMassData(); //I think its safe if I just redo this after setting material and shape
//	objVerts = shape->GetPoints();
//	numVerts = objVerts.size(); //TODO
//	for (int i = 0; i < numVerts; i++) {
//		worldVerts.push_back(Vector2f(0.0f, 0.0f));
//		faceNorms.push_back(Vector2f(0.0f, 0.0f));
//	}
//	objVerts = shape->GetPoints();
//	UpdateVertsAndNorms();
//}

void RigidBody::SetMassData()
{
	float area = shape->GetArea();
	float inertiaCoeff = shape->GetInertiaCoeff();
	float mass = area * mat.dens;
	massD.SetMass(mass);
	massD.SetInertia(mass * inertiaCoeff);
}

void RigidBody::RotateVerts()
{//this must be called every time orientation is changed
	Eigen::Rotation2D<float> rotation(transform.orient - transform.prevOrient);
	Matrix2f rotationMatrix = rotation.toRotationMatrix();
	int size = objVerts.size();
	for (int i = 0; i < size; ++i) {
		objVerts[i] = rotationMatrix * objVerts[i];
	}
}

void RigidBody::UpdateVertsAndNorms()
{
	//mtx.lock();
	VertsToWorldSpace();
	UpdateFaceNorms();
	//mtx.unlock();
}

void RigidBody::UpdateFaceNorms()
{ //TODO
	//std::vector<Vector2f> faceNormals;
	if (shape->GetType() == Shape::ShapeType::RECTANGLE) {
		faceNorms[0] = (Vector2f(objVerts[1] - objVerts[0]).unitOrthogonal()); 	//top face normal
		faceNorms[1] = (Vector2f(objVerts[2] - objVerts[1]).unitOrthogonal()); 	//left face normal
		faceNorms[2] = (Vector2f(objVerts[3] - objVerts[2]).unitOrthogonal()); 	//right face normal
		faceNorms[3] = (Vector2f(objVerts[0] - objVerts[3]).unitOrthogonal()); 	//bottom face normal
	}
	else if (shape->GetType() == Shape::ShapeType::CIRCLE) {
		//Do nothing this should return an empty list representing the infinite/nonexistent face normals of a circle
	}
	else if (shape->GetType() == Shape::ShapeType::POLYGON) {
		int size = objVerts.size();
		for (int i = 0; i < size; i++) {
			if (i < size - 1) {//might be backwards but idk
				faceNorms[i] = Vector2f(objVerts[i + 1] - objVerts[i]).unitOrthogonal();
			}
			else {
				faceNorms[i] = Vector2f(objVerts[0] - objVerts[i]).unitOrthogonal();
			}
		}
	}
}

void RigidBody::VertsToWorldSpace()
{
	int size = worldVerts.size();
	for (int i = 0; i < size; i++) {
		//Vector2f worldSpacePoint = transform.pos + objVerts[i];
		worldVerts[i] = Vector2f(transform.pos + objVerts[i]);
	}
}

sf::VertexArray RigidBody::CreatOrientationLine(float i_lerp_fraction) {
	Vector2f lerpPos = GetLerpPosition(i_lerp_fraction);
	float lerpOrient = GetLerpOrient(i_lerp_fraction);
	sf::VertexArray lines = sf::VertexArray(sf::LinesStrip, 2);
	sf::Vector2f position1 = sf::Vector2f(lerpPos(0), lerpPos(1));
	Eigen::Rotation2D<float> rotation(lerpOrient);
	Vector2f edgeOffsetFromCenter = Vector2f(shape->GetDistToEdge(), 0.0f);
	Matrix2f rotationMatrix = rotation.toRotationMatrix();
	Vector2f edgeoffSetRotated = rotationMatrix * edgeOffsetFromCenter;
	Vector2f edgePoint = transform.pos + edgeoffSetRotated;
	sf::Vector2f edgePointSFML = sf::Vector2f(edgePoint(0), edgePoint(1));
	lines[0] = sf::Vertex(position1, sf::Color::Red);
	lines[1] = sf::Vertex(edgePointSFML, sf::Color::Red);
	return lines;
}

std::vector<sf::CircleShape> RigidBody::CreateStructurePoints(float i_lerp_fraction)
{
	std::vector<Vector2f> worldPoints = GetVertCords();
	std::vector<sf::CircleShape> drawables;
	for (Vector2f point : worldPoints) {
		sf::CircleShape pointRepr(3.0f);
		//Circles are drawn starting at the top left corner of their bounding box,
		//so you need to set the center to +1 radius in the X and Y
		pointRepr.setOrigin(3.0f, 3.0f);
		pointRepr.setPosition(point(0), point(1));
		pointRepr.setFillColor(sf::Color::Red);
		drawables.push_back(pointRepr);
	}
	return drawables;
}

std::vector<Vector2f> RigidBody::GetVertCords() {
	return worldVerts;
}

std::vector<Vector2f> RigidBody::GetFaceNorms()
{ 
	return faceNorms;
}

Vector2f RigidBody::GetInstVel()
{
	return vel + force;
}

float RigidBody::GetInstAngVel()
{
	return angVel + torq;
}

void RigidBody::ApplyImpulse(Vector2f i_imp, Vector2f contactP) 
{ 
	force  += i_imp * massD.GetMassInv();
	//float oo = Math::CrossProdScalar(contactP, i_imp);
	torq += Math::CrossProdScalar(contactP, i_imp) * massD.GetInertInv() * ANGULAR_VELOCITY_ADJUSTMENT;
}

void RigidBody::AdjustPosition(Vector2f i_adjustment)
{
	transform.pos += i_adjustment;
	UpdateVertsAndNorms();
}

void RigidBody::ResetPosition(Vector2f i_newPos)
{
	transform.pos = i_newPos;
	UpdateVertsAndNorms();
}

void RigidBody::ResetOrientation(float i_orient)
{
	transform.prevOrient = transform.orient;
	transform.orient = i_orient;
	RotateVerts();
}

void RigidBody::ResetOrientation(Vector2f i_dirVect)
{
	transform.prevOrient = transform.orient;
	transform.orient = atan2(i_dirVect[1], i_dirVect[0]);
	RotateVerts();
}

void RigidBody::IntegrateForces()
{
	if (ignoreForcesThisStep) {
		force = Vector2f(0.0f, 0.0f);
		torq = 0.0f;
		ignoreForcesThisStep = false;
	}
	else {
		vel += force;
		angVel += torq;
		//vel += GRAVITY_COEFFICIENT * massD.GetMassInv();
		force = Vector2f(0.0f, 0.0f);
		torq = 0.0f;	
	}
}

void RigidBody::IntegrateVelocity(float i_deltaTime)
{
	transform.prevPos = transform.pos;
	transform.prevOrient = transform.orient;
	//Need to rotate points here

	transform.pos += vel * (i_deltaTime / AVG_MILLISEC_PER_UPDATE);
	transform.orient += angVel * (i_deltaTime / AVG_MILLISEC_PER_UPDATE);
	RotateVerts();
	UpdateVertsAndNorms();
	//GLOBAL DECELERATION
	vel = vel - (vel * GLOBAL_DECELERATION_LINEAR * (i_deltaTime / AVG_MILLISEC_PER_UPDATE));
	angVel = angVel - (angVel * GLOBAL_DECELERATION_ANGULAR * (i_deltaTime / AVG_MILLISEC_PER_UPDATE));

}

Vector2f RigidBody::GetLerpPosition(float i_lerp_fraction)
{
	return (1.0f - i_lerp_fraction) * transform.prevPos + i_lerp_fraction * transform.pos;
}

float RigidBody::GetLerpOrient(float i_lerp_fraction)
{
	return (1.0f - i_lerp_fraction) * transform.prevOrient + i_lerp_fraction * transform.orient;
}

void RigidBody::ChangeSizeOfShape(float i_widthOrRadius, float i_heightOrGarbage)
{
	shape->ChangeSizeOfShape(i_widthOrRadius, i_heightOrGarbage);
	SetMassData();
	objVerts = shape->GetPoints();
	UpdateVertsAndNorms();
}
