/*
This physics engine was designed based on Randy Gaul's tutorial found here:
https://gamedevelopment.tutsplus.com/tutorials/how-to-create-a-custom-2d-physics-engine-oriented-rigid-bodies--gamedev-8032
*/

#include "Physics.h"


Physics::Physics()
{
}

Physics::~Physics()
{
}

Vector2f Physics::GetMaxProjection(Vector2f i_dir, std::vector<Vector2f> i_vertices)
{
	Vector2f maxProjVertex = Vector2f(0, 0);
	float maxProj = -std::numeric_limits<float>::max();
	for (Vector2f vertex : i_vertices) {
		float currProj = vertex.dot(i_dir);
		if (currProj > maxProj) {
			maxProjVertex = vertex;
			maxProj = currProj;
		}
	}
	return maxProjVertex;
}

std::vector<Vector2f> Physics::FindIncidentFace(RigidBody * i_refEnt, RigidBody * i_incidentEnt, int i_refIndex)
{
	Vector2f referenceNormal = i_refEnt->GetFaceRectNormals()[i_refIndex];
	std::vector<Vector2f> incidentVerts = i_incidentEnt->GetVertexCoords();
	std::vector<Vector2f> incidentNormals = i_incidentEnt->GetFaceRectNormals();
	//ignoring world to model space stuff right here
	int incidentFaceIndex = -1;
	float minProjection = std::numeric_limits<float>::max();
	for (int i = 0; i < incidentVerts.size(); ++i) {
		//why am in taking the projection onto the normal, figure this out later
		float currProj = referenceNormal.dot(incidentNormals[i]);
		if (currProj < minProjection) {
			minProjection = currProj;
			incidentFaceIndex = i;
		}
	}
	//assign face vertices for incident face;
	Vector2f incidentFaceVert1 = incidentVerts[incidentFaceIndex];
	Vector2f incidentFaceVert2 = incidentVerts[(incidentFaceIndex + 1) % incidentVerts.size()];
	std::vector<Vector2f> incidentFaceVerts = { incidentFaceVert1 , incidentFaceVert2 };
	return incidentFaceVerts;
}

std::vector<float> Physics::FindAxisLeastPenetration(RigidBody* i_entA, RigidBody* i_entB)
{
	std::vector<Vector2f> verticesA = i_entA->GetVertexCoords();
	std::vector<Vector2f> verticesB = i_entB->GetVertexCoords();
	std::vector<Vector2f> normalsA = i_entA->GetFaceRectNormals();
	float maxPenetrationDist = -std::numeric_limits<float>::max();
	float maxPenetrationVert = -1;
	for (int i = 0; i < verticesA.size(); ++i) {
		Vector2f bMaxVertProjAlongNormalA = GetMaxProjection(-normalsA[i], verticesB);
		//Not converting between object spaces, I really hope it wont break it to just do everything in world space for now
		Vector2f distFromConerAToBProj = bMaxVertProjAlongNormalA - verticesA[i];
		float penetrationDist = normalsA[i].dot(distFromConerAToBProj);
		if (penetrationDist > maxPenetrationDist) {
			maxPenetrationDist = penetrationDist;
			maxPenetrationVert = i;
		}
	}
	return std::vector<float> { maxPenetrationDist, maxPenetrationVert };
}

bool Physics::ResolveCircleToCircleCollision(CollisionData* i_data)
{
	std::shared_ptr<Circle> circlePtr1 = std::dynamic_pointer_cast<Circle>(i_data->entPtr1->GetShape());
	std::shared_ptr<Circle> circlePtr2 = std::dynamic_pointer_cast<Circle>(i_data->entPtr2->GetShape());

///////////////

////////////////////////IDK HOW THIS IS GONNA WORK OUT
	Vector2f e1CenterPt = i_data->entPtr1->transform.position;
	Vector2f e2CenterPt = i_data->entPtr2->transform.position;
	float rSqaured = pow((circlePtr1->GetRadius() + circlePtr2->GetRadius()), 2);
	Vector2f collisionNormal = e2CenterPt - e1CenterPt;
	if (rSqaured > collisionNormal.squaredNorm()) {
		i_data->normal = collisionNormal.normalized();
		return true;
	}
	return false;
}

bool Physics::ResolveRectToCircleCollision(CollisionData* i_data)
{  //Entity 1 is rectangle and entity 2 is circle
	RigidBody* rectEnt = i_data->entPtr1;
	RigidBody* circleEnt = i_data->entPtr2;
	//std::shared_ptr<Rectangle> rectPtr = std::dynamic_pointer_cast<Rectangle>(i_data->entPtr1->GetShape());
	std::shared_ptr<Circle> circlePtr = std::dynamic_pointer_cast<Circle>(i_data->entPtr2->GetShape());

	float maxSeperation = -std::numeric_limits<float>::max();
	int collisionFaceIndex = -1;
	std::vector<Vector2f> rectVertices = rectEnt->GetVertexCoords();
	std::vector<Vector2f> normals = rectEnt->GetFaceRectNormals();
	for (int i = 0; i < normals.size(); ++i) {
		float currSeperation = normals[i].dot(circleEnt->transform.position - rectVertices[i]);
		if (currSeperation > circlePtr->GetRadius()) {
			return false;
		}
		if (currSeperation > maxSeperation) {
			maxSeperation = currSeperation;
			collisionFaceIndex = i;
		}
	}

	Vector2f rectVert1 = rectVertices[collisionFaceIndex];
	Vector2f rectVert2 = rectVertices[(collisionFaceIndex + 1) % rectVertices.size()];

	if (maxSeperation < 0) {
		//The tutorial applied a transition but I think it was to just get everythig in world space so Im gonna leave it
		i_data->normal = (normals[collisionFaceIndex]);
		std::cout << "COLLISION";
		return true;
	}

	//Rest of code it to determine veroni region 
	float dotProd1 = (circleEnt->transform.position - rectVert1).dot(rectVert2 - rectVert1);
	float dotProd2 = (circleEnt->transform.position - rectVert2).dot(rectVert1 - rectVert2);


	// Circle center lies beyond face region, past V1
	if (dotProd1 <= 0.0f) {
		if ((circleEnt->transform.position - rectVert1).squaredNorm() > pow(circlePtr->GetRadius(), 2)) { return true; }
		Vector2f veroniNormal = rectVert1 - circleEnt->transform.position;
		veroniNormal.normalize();
		i_data->normal = veroniNormal;
	}
	// Circle center lies beyond face region, past V2
	else if (dotProd2 <= 0.0f) {
		if ((circleEnt->transform.position - rectVert2).squaredNorm() > pow(circlePtr->GetRadius(), 2)) { return true; }
		//ignorign some more stuff that I think is just translation to world space which Ive already done
		Vector2f veroniNormal = rectVert2 - circleEnt->transform.position;
		veroniNormal.normalize();
		i_data->normal = veroniNormal;
	}
	// Circle center is within face region
	else {
		Vector2f normal = normals[collisionFaceIndex];
		if ((circleEnt->transform.position - rectVert1).dot(normal) > circlePtr->GetRadius()) { return true; }
		//normal = -normal;
		i_data->normal = normal;
	}
	std::cout << "COLLISION";
	return true;
}

bool Physics::ResolveRectToRectCollision(CollisionData* i_data) {
	std::vector<float> penetrationA = FindAxisLeastPenetration(i_data->entPtr1, i_data->entPtr2);
	if (penetrationA[0] > 0.0f) {
		return false;
	}
	std::vector<float> penetrationB = FindAxisLeastPenetration(i_data->entPtr2, i_data->entPtr1);
	if (penetrationB[0] > 0.0f) {
		return false;
	}

	int refIndex;
	bool flip;
	RigidBody* refEnt;
	RigidBody* incidentEnt;

	if (penetrationA > penetrationB) {
		refEnt = i_data->entPtr1;
		incidentEnt = i_data->entPtr1;
		refIndex = penetrationA[1];
		flip = false;
	}
	else {
		refEnt = i_data->entPtr2;
		incidentEnt = i_data->entPtr1;
		refIndex = penetrationB[1];
		flip = true;
	}

	std::vector<Vector2f> incidentFaceVerts = FindIncidentFace(refEnt, incidentEnt, refIndex);
	std::vector<Vector2f> refVerts = refEnt->GetVertexCoords();

	Vector2f refEntV1 = refVerts[refIndex];
	Vector2f refEntV2 = refVerts[(refIndex + 1) % refVerts.size()];

	//ignoring local to world transition

	Vector2f refSidePlaneNormal = refEntV2 - refEntV1;
	refSidePlaneNormal.normalize();
	//need to orthogonalize, not sure if this will be in the right direction, will flip this negative if shit sucks
	Vector2f refFaceNormal = refSidePlaneNormal.unitOrthogonal();

	i_data->normal = flip ? -refFaceNormal : refFaceNormal;
	std::cout << "COLLISION";
	return true;

	//the rest of the tutorial involves clipping and I dont need that until I start using friction, gonna just stop here
	//TODO: finish this when I start using friction
	//FROM TUTE because idk what this equation is used for 
	// ax + by = c
	// c is distance from origin
}

void Physics::CheckCollision(RigidBody * i_ent1, RigidBody * i_ent2)
{
	CollisionData collisionData = CollisionData();
	collisionData.entPtr1 = i_ent1;
	collisionData.entPtr2 = i_ent2;
	Shape::ShapeType shape1 = collisionData.entPtr1->GetShape()->GetShapeType();
	Shape::ShapeType shape2 = collisionData.entPtr2->GetShape()->GetShapeType();
	bool collisionOccured = false;
	if ((shape1 == Shape::Circle) && (shape2 == Shape::Circle))
	{
		collisionOccured = ResolveCircleToCircleCollision(&collisionData);
	}
	else if ((shape1 == Shape::Rectangle) && (shape2 == Shape::Rectangle))
	{
		collisionOccured = ResolveRectToRectCollision(&collisionData);
	}
	else if ((shape1 == Shape::Rectangle) && (shape2 == Shape::Circle))
	{
		collisionOccured = ResolveRectToCircleCollision(&collisionData);
	}
	else if ((shape1 == Shape::Circle) && (shape2 == Shape::Rectangle))
	{
		collisionData.entPtr1 = i_ent2;
		collisionData.entPtr2 = i_ent1;
		collisionOccured = ResolveRectToCircleCollision(&collisionData);
	}
	if (collisionOccured) {
		ApplyCollisionImpulse(&collisionData);
	}
	return;
}

//void Physics::ApplyCollisionImpulse(CollisionData* i_data) {
//	Vector2f relativeVelocity = i_data->entPtr2->GetVelocity() - i_data->entPtr1->GetVelocity();
//	float  VelocityAlongNormal = relativeVelocity.dot(i_data->normal);
//	if (VelocityAlongNormal > 0) { return; }
//	//defaulting restitution to .1 for now
//	float j = -1.1f * VelocityAlongNormal;
//	float impulseScalar = j / (i_data->entPtr1->massData.GetMassInv() + i_data->entPtr2->massData.GetMassInv());
//	Vector2f impulse = impulseScalar * i_data->normal;
//	i_data->entPtr1->ApplyImpulse(-impulse);
//	i_data->entPtr2->ApplyImpulse(impulse);
//}

void Physics::ApplyCollisionImpulse(CollisionData* i_data) {
	Vector2f relativeVelocity = i_data->entPtr2->GetVelocity() - i_data->entPtr1->GetVelocity();
	float  VelocityAlongNormal = relativeVelocity.dot(i_data->normal);
	if (VelocityAlongNormal > 0) { return; }
	//defaulting restitution to .1 for now
	float j = -1.1f * VelocityAlongNormal;
	float inverseMassSum = (i_data->entPtr1->massData.GetMassInv() + i_data->entPtr2->massData.GetMassInv());
	float impulseScalar = inverseMassSum == 0 ? 0 : (j / inverseMassSum);
	Vector2f impulse = impulseScalar * i_data->normal;
	i_data->entPtr1->ApplyImpulse(-impulse);
	i_data->entPtr2->ApplyImpulse(impulse);
}



//void Physics::TimeStepRigidBody(RigidBody * i_entity, float i_stepSize)
//{
//	InegrateVelocity();
//
//
//
//	///position
//
//
//	///rotation
//	//dont have inertia values yet, everything is hard set to 2
//	float angularVelocity = i_entity->GetRotation() + ((1.0f / 2.0f) * i_entity->torque);
//	i_entity->SetRotation(angularVelocity);
//
//	///testing
//	//entity->SetRotation(entity->GetRotation() + 0.01f);
//	//things slow down to approximate top down view where gravity+friction is slowing them down
//	i_entity->torque = (i_entity->torque * GLOBAL_DECELERATION);
//	i_entity->SetVelocity(Velocity * GLOBAL_DECELERATION);
//}
