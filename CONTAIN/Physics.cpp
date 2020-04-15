//https://gamedevelopment.tutsplus.com/tutorials/how-to-create-a-custom-2d-physics-engine-the-basics-and-impulse-resolution--gamedev-6331

#include "Physics.h"


Physics::Physics()
{
}

Physics::~Physics()
{
}

void Physics::TestCollision(RigidBody * i_ent1, RigidBody * i_ent2)
{
	CollisionData collisionData = CollisionData();
	collisionData.entPtr1 = i_ent1;
	collisionData.entPtr2 = i_ent2;
	Shape::ShapeType shape1 = collisionData.entPtr1->GetShape()->GetShapeType();
	Shape::ShapeType shape2 = collisionData.entPtr2->GetShape()->GetShapeType();
	bool collisionOccured = false;
	if ((shape1 == Shape::Circle) && (shape2 == Shape::Circle))
	{
		collisionOccured = ResolveCircleCollision(&collisionData);
	}
	else if ((shape1 == Shape::Rectangle) && (shape2 == Shape::Rectangle))
	{
		collisionOccured = ResolveRectangleCollision(&collisionData);
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
		ApplyImpulse(&collisionData);
	}
	return;
}

void Physics::MoveEntity(RigidBody * entity, float i_stepSize)
{
	///position
	Vector2f oldPos = entity->GetTransform()->GetPosition();
	Vector2f Velocity = entity->GetVelocity();
	Vector2f newPos = Vector2f(oldPos(0) + ((i_stepSize / 100) * Velocity(0)), oldPos(1) + ((i_stepSize / 100) * Velocity(1)));
	entity->GetTransform()->SetPosition(newPos);

	///rotation
	//dont have inertia values yet, everything is hard set to 2
	float angularVelocity = entity->GetRotation() + ((1.0f / 2.0f) * entity->torque);
	entity->SetRotation(angularVelocity);

	///testing
	//entity->SetRotation(entity->GetRotation() + 0.01f);
	//things slow down to approximate top down view where gravity+friction is slowing them down
	entity->torque = (entity->torque * 0.995f);
	entity->SetVelocity(Velocity * 0.995f);
}

bool Physics::ResolveCircleCollision(CollisionData* i_data)
{
	std::shared_ptr<Circle> circlePtr1 = std::dynamic_pointer_cast<Circle>(i_data->entPtr1->GetShape());
	std::shared_ptr<Circle> circlePtr2 = std::dynamic_pointer_cast<Circle>(i_data->entPtr2->GetShape());
	Vector2f e1CenterPt = i_data->entPtr1->GetCenterPoint();
	Vector2f e2CenterPt = i_data->entPtr2->GetCenterPoint();
	float rSqaured = pow((circlePtr1->GetRadius() + circlePtr2->GetRadius()), 2);
	Vector2f collisionNormal = e2CenterPt - e1CenterPt;
	if (rSqaured > collisionNormal.squaredNorm()) {
		i_data->normal = collisionNormal.normalized();
		return true;
	}
	return false;
}

//bool Physics::ResolveRectangleCollision(CollisionData* i_data)
//{
//	std::vector<Vector2f> ent1WorldCoords = i_data->entPtr1->GetVertexCoords();
//	std::vector<Vector2f> ent2WorldCoords = i_data->entPtr2->GetVertexCoords();
//	//creating axisses for rect 1, We only need top and left because the otheres are parallel
//	std::vector<Vector2f> ent1FacesAxis;
//	ent1FacesAxis.push_back(Vector2f(ent1WorldCoords[1] - ent1WorldCoords[0]).unitOrthogonal()); 	//top face normal
//	ent1FacesAxis.push_back(Vector2f(ent1WorldCoords[2] - ent1WorldCoords[0]).unitOrthogonal()); 	//left face normal
//	//creating axisses for rect 2, We only need top and left because the otheres are parallel
//	std::vector<Vector2f> ent2FacesAxis;
//	ent2FacesAxis.push_back(Vector2f(ent2WorldCoords[1] - ent2WorldCoords[0]).unitOrthogonal()); 	//top face normal
//	ent2FacesAxis.push_back(Vector2f(ent2WorldCoords[2] - ent2WorldCoords[0]).unitOrthogonal()); 	//left face normal
//
//	bool collisionAlongBox1Normals = CheckForCollisionAlongNormals(ent1FacesAxis, ent1WorldCoords, ent2WorldCoords);
//	bool collisionAlongBox2Normals = CheckForCollisionAlongNormals(ent2FacesAxis, ent1WorldCoords, ent2WorldCoords);
//
//	if (collisionAlongBox1Normals && collisionAlongBox2Normals) {
//		std::cout << "COLLISION";
//		return true;
//	}
//}

//
//bool Physics::ResolveRectToCircleCollision(CollisionData* i_data)
//{  //Entity 1 is rectangle and entity 2 is circle
//	Vector2f rectCenter = i_data->entPtr1->GetCenterPoint();
//	Vector2f circleCenter = i_data->entPtr2->GetCenterPoint();
//	std::shared_ptr<Rectangle> rectPtr = std::dynamic_pointer_cast<Rectangle>(i_data->entPtr1->GetShape());
//	std::shared_ptr<Circle> circlePtr = std::dynamic_pointer_cast<Circle>(i_data->entPtr2->GetShape());
//	Vector2f distance = circleCenter - rectCenter; 
//	//Closest point on rectangle to center of circle
//	float halfWidth = (rectPtr->GetWidth() / 2.0f);
//	float halfHeight = (rectPtr->GetHeight() / 2.0f);
//	float closestPointOnRectX = std::min(std::max(distance(0), -halfWidth), halfWidth);
//	float closestPointOnRectY = std::min(std::max(distance(1), -halfHeight), halfHeight);
//	Vector2f closestPointOnRect = Vector2f(closestPointOnRectX, closestPointOnRectY);
//	bool circleInsideRect = false;
//	if (distance == closestPointOnRect) {
//		circleInsideRect = true;
//		if (abs(distance(0)) > abs(distance(0))) {//if y distance is shorter than x distance
//			if (closestPointOnRect(0) > 0.0f) { closestPointOnRect(0) = halfWidth;  }
//			else { closestPointOnRect(0) = -halfWidth; }
//		}
//		else {
//			if (closestPointOnRect(1) > 0.0f) { closestPointOnRect(1) = halfHeight; }
//			else { closestPointOnRect(1) = -halfHeight; }
//		}
//	}
//	Vector2f collisionNormal = distance - closestPointOnRect;
//	if (collisionNormal.squaredNorm() < pow(circlePtr->GetRadius(), 2)) { 
//		//flip collision normal if circle was inside rect 
//		if (circleInsideRect) {collisionNormal = -collisionNormal; }
//		else { i_data->normal = collisionNormal.normalized(); }
//		i_data->normal = collisionNormal.normalized();
//		//not sure what to do with penetration right now but Im supposed to set it here
//		return true;
//	}
//	return false;
//}


bool Physics::ResolveRectToCircleCollision(CollisionData* i_data)
{  //Entity 1 is rectangle and entity 2 is circle
	RigidBody* rectEnt = i_data->entPtr1;
	RigidBody* circleEnt = i_data->entPtr2;
	//std::shared_ptr<Rectangle> rectPtr = std::dynamic_pointer_cast<Rectangle>(i_data->entPtr1->GetShape());
	std::shared_ptr<Circle> circlePtr = std::dynamic_pointer_cast<Circle>(i_data->entPtr2->GetShape());
	
	float maxSeperation = -std::numeric_limits<float>::max();
	int collisionFaceIndex = -1;
	std::vector<Vector2f> rectVertices = rectEnt->GetVertexCoords();
	std::vector<Vector2f> normals = rectEnt->GetFaceNormals();
	for (int i = 0; i < normals.size(); ++i) {
		float currSeperation = normals[i].dot(circleEnt->GetTransform()->GetPosition() - rectVertices[i]);
		if (currSeperation > circlePtr->GetRadius()) {
			return false;
		}
		if (currSeperation > maxSeperation) {
			maxSeperation = currSeperation;
			collisionFaceIndex = i;
		}
	}

	Vector2f rectVert1 = rectVertices[collisionFaceIndex];
	Vector2f rectVert2 = rectVertices[(collisionFaceIndex+1)%rectVertices.size()];

	if (maxSeperation < 0) {
		//The tutorial applied a transition but I think it was to just get everythig in world space so Im gonna leave it
		i_data->normal = (normals[collisionFaceIndex]);
		std::cout << "COLLISION";
		return true;
	}

	//Rest of code it to determine veroni region 
	float dotProd1 = (circleEnt->GetTransform()->GetPosition() - rectVert1).dot(rectVert2 - rectVert1);
	float dotProd2 = (circleEnt->GetTransform()->GetPosition() - rectVert2).dot(rectVert1 - rectVert2);


	// Circle center lies beyond face region, past V1
	if (dotProd1 <= 0.0f) {
		if ((circleEnt->GetTransform()->GetPosition() - rectVert1).squaredNorm() > pow(circlePtr->GetRadius(), 2)) { return true; }
		Vector2f veroniNormal = rectVert1 - circleEnt->GetTransform()->GetPosition();
		veroniNormal.normalize();
		i_data->normal = veroniNormal;
	}
	// Circle center lies beyond face region, past V2
	else if (dotProd2 <= 0.0f) {
		if ((circleEnt->GetTransform()->GetPosition() - rectVert2).squaredNorm() > pow(circlePtr->GetRadius(), 2)) { return true;  }
		//ignorign some more stuff that I think is just translation to world space which Ive already done
		Vector2f veroniNormal = rectVert2 - circleEnt->GetTransform()->GetPosition();
		veroniNormal.normalize();
		i_data->normal = veroniNormal;
	}
	// Circle center is within face region
	else {
		Vector2f normal = normals[collisionFaceIndex];
		if ((circleEnt->GetTransform()->GetPosition() - rectVert1).dot(normal) > circlePtr->GetRadius()) { return true;  }
		//normal = -normal;
		i_data->normal = normal;
	}
	std::cout << "COLLISION";
	return true;
}

bool Physics::ResolveRectangleCollision(CollisionData* i_data) {
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
	Vector2f refEntV2 = refVerts[(refIndex+1)%refVerts.size()];

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


std::vector<Vector2f> Physics::FindIncidentFace(RigidBody * i_refEnt, RigidBody * i_incidentEnt, int i_refIndex)
{
	Vector2f referenceNormal = i_refEnt->GetFaceNormals()[i_refIndex];
	std::vector<Vector2f> incidentVerts = i_incidentEnt->GetVertexCoords();
	std::vector<Vector2f> incidentNormals = i_incidentEnt->GetFaceNormals();
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

std::vector<float> Physics::FindAxisLeastPenetration(RigidBody* i_entA, RigidBody* i_entB)
{
	std::vector<Vector2f> verticesA = i_entA->GetVertexCoords();
	std::vector<Vector2f> verticesB = i_entB->GetVertexCoords();
	std::vector<Vector2f> normalsA = i_entA->GetFaceNormals();
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

bool Physics::CheckForCollisionAlongNormals(std::vector<Vector2f> normalVect, std::vector<Vector2f> vertexVect1, std::vector<Vector2f> vertexVect2) {

	for (int i = 0; i < normalVect.size(); ++i) {
		float min_proj_box1 = vertexVect1[0].dot(normalVect[i]);
		float min_dot_box1 = 0;
		float max_proj_box1 = vertexVect1[0].dot(normalVect[i]);
		float max_dot_box1 = 0;

		for (int j = 1; j < vertexVect1.size(); ++j) {
			float projection = vertexVect1[j].dot(normalVect[i]);
			if (projection < min_proj_box1) {
				min_proj_box1 = projection;
				min_dot_box1 = j;
			}
			else if (projection > max_proj_box1) {
				max_proj_box1 = projection;
				max_dot_box1 = j;
			}
		}

		float min_proj_box2 = vertexVect2[0].dot(normalVect[i]);
		float min_dot_box2 = 0;
		float max_proj_box2 = vertexVect2[0].dot(normalVect[i]);
		float max_dot_box2 = 0;

		for (int j = 1; j < vertexVect2.size(); ++j) {
			float projection = vertexVect2[j].dot(normalVect[i]);
			if (projection < min_proj_box2) {
				min_proj_box2 = projection;
				min_dot_box2 = j;
			}
			else if (projection > max_proj_box2) {
				max_proj_box2 = projection;
				max_dot_box2 = j;
			}
		}

		bool seperatedAxis = (max_proj_box2 < min_proj_box1) || (max_proj_box1 < min_proj_box2);
		if (seperatedAxis) {
			return false;
		}//NO Collision
	}
	return true;
}

void Physics::ApplyImpulse(CollisionData* i_data) {
	Vector2f relativeVelocity = i_data->entPtr2->GetVelocity() - i_data->entPtr1->GetVelocity();
	float  VelocityAlongNormal = relativeVelocity.dot(i_data->normal);
	//float  VelocityAlongNormal = i_data->normal.dot(relativeVelocity);
	if (VelocityAlongNormal > 0) { return; }
	//ignoring restitution for now
	//calculating impulse scalar
	float j = -1.1f * VelocityAlongNormal;
	//Approximating mass
	float impulseScalar = j / (1.0f);
	Vector2f impulse = impulseScalar * i_data->normal;
	Vector2f newVel1 = i_data->entPtr1->GetVelocity() - ((1.0f / 2.0f) * impulse);
	Vector2f newVel2 = i_data->entPtr2->GetVelocity() + ((1.0f / 2.0f) * impulse);
	i_data->entPtr1->SetVelocity(newVel1);
	i_data->entPtr2->SetVelocity(newVel2);
}
