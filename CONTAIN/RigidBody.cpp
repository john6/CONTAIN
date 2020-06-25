#include "RigidBody.h"

RigidBody::RigidBody(std::shared_ptr<Shape> i_shape, Material i_material) :
	shape{ i_shape }, mat{ i_material }, transform(), massD(),
	vel{Vector2f(0.0f ,0.0f)}, force{Vector2f(0.0f ,0.0f)},
	angVel{0.0f}, torq{0.0f}
{
	SetMassData();
	objVerts = shape->GetPoints();
	UpdateVertsAndNorms();
}

RigidBody::~RigidBody()
{
}

void RigidBody::SetMassData()
{
	float area = shape->GetArea();
	float inertiaCoeff = shape->GetInertiaCoeff();
	float mass = area * mat.dens;
	massD.SetMass(mass);
	massD.SetInertia(mass * inertiaCoeff);
}

//std::unique_ptr<sf::Shape> RigidBody::CreateDrawable(float i_lerp_fraction) {
//	//for linear interpolation this will now use the previous position and orientation
//	Vector2f lerpPos = GetLerpPosition(i_lerp_fraction);
//	float lerpOrient = GetLerpOrient(i_lerp_fraction);
//	std::unique_ptr<sf::Shape> drawShape = shape->GetSFMLRepr();
//	drawShape->setOrigin(sf::Vector2f(shape->GetSFMLOriginOffset()(0), shape->GetSFMLOriginOffset()(1)));
//	drawShape->setPosition(lerpPos(0), lerpPos(1));
//	drawShape->setRotation((lerpOrient*180.0f)/PI);
//	drawShape->setOutlineColor(sf::Color::White);
//	drawShape->setOutlineThickness(3.0f);          //setOutlineColor(sf::Color::White);
//	drawShape->setFillColor(sf::Color::Black); //TODO: Colors
//	return drawShape;
//}

std::vector<Vector2f> RigidBody::RotatePoints(std::vector<Vector2f> i_axisAlignedCoords)
{
	Eigen::Rotation2D<float> rotation(transform.orient);
	Matrix2f rotationMatrix = rotation.toRotationMatrix();
	for (int i = 0; i < i_axisAlignedCoords.size(); ++i) {
		i_axisAlignedCoords[i] = rotationMatrix * i_axisAlignedCoords[i];
	}
	return i_axisAlignedCoords;
}

void RigidBody::UpdateVertsAndNorms()
{
	worldVerts = vertsToWorldSpace(objVerts);
	UpdateFaceNorms();
}

void RigidBody::UpdateFaceNorms()
{
	std::vector<Vector2f> faceNormals;
	if (shape->GetType() == Shape::ShapeType::Rectangle) {
		std::vector<Vector2f> rotatedPoints = RotatePoints(objVerts);
		faceNormals.push_back(Vector2f(rotatedPoints[1] - rotatedPoints[0]).unitOrthogonal()); 	//top face normal
		faceNormals.push_back(Vector2f(rotatedPoints[2] - rotatedPoints[1]).unitOrthogonal()); 	//left face normal
		faceNormals.push_back(Vector2f(rotatedPoints[3] - rotatedPoints[2]).unitOrthogonal()); 	//right face normal
		faceNormals.push_back(Vector2f(rotatedPoints[0] - rotatedPoints[3]).unitOrthogonal()); 	//bottom face normal
	}
	else if (shape->GetType() == Shape::ShapeType::Circle) {
		//Do nothing this should return an empty list representing the infinite/nonexistent face normals of a circle
	}
	faceNorms = faceNormals;
}

std::vector<Vector2f> RigidBody::vertsToWorldSpace(std::vector<Vector2f> i_objectSpaceCoords)
{
	std::vector<Vector2f> worldSpaceCoords;
	std::vector<Vector2f> rotatedPoints = RotatePoints(i_objectSpaceCoords);
	for (Vector2f rotatedCoord : rotatedPoints) {
		Vector2f worldSpacePoint = transform.pos + rotatedCoord;
		worldSpaceCoords.push_back(worldSpacePoint);
	}
	return worldSpaceCoords;
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

void RigidBody::IntegrateForces()
{
	vel += force;
	angVel += torq;
	//vel += GRAVITY_COEFFICIENT * massD.GetMassInv();
	vel *= GLOBAL_DECELERATION_LINEAR;
	angVel *= GLOBAL_DECELERATION_ANGULAR;
	force = Vector2f(0.0f, 0.0f);
	torq = 0.0f;
}

void RigidBody::IntegrateVelocity(float i_deltaTime)
{
	transform.prevPos = transform.pos;
	transform.prevOrient = transform.orient;

	transform.pos += vel * (i_deltaTime / AVG_MILLISEC_PER_UPDATE);
	transform.orient += angVel * (i_deltaTime / AVG_MILLISEC_PER_UPDATE);
	UpdateVertsAndNorms();
}

Vector2f RigidBody::GetLerpPosition(float i_lerp_fraction)
{
	return (1.0f - i_lerp_fraction) * transform.prevPos + i_lerp_fraction * transform.pos;
}

float RigidBody::GetLerpOrient(float i_lerp_fraction)
{
	return (1.0f - i_lerp_fraction) * transform.prevOrient + i_lerp_fraction * transform.orient;
}
