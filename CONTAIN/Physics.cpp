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
	Vector2f referenceNormal = i_refEnt->GetFaceNorms()[i_refIndex];
	std::vector<Vector2f> incidentVerts = i_incidentEnt->GetVertCords();
	std::vector<Vector2f> incidentNormals = i_incidentEnt->GetFaceNorms();
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

int Physics::Clip(Vector2f normal, float c, std::vector<Vector2f>* face)
{
	int sp = 0;
	std::vector<Vector2f> out = *face;
	float d1 = normal.dot(out[0]) - c;
	float d2 = normal.dot(out[1]) - c;

	//clips behind plane
	if (d1 <= 0.0f) { out[sp++] = face->at(0); }
	if (d2 <= 0.0f) { out[sp++] = face->at(1); }

	//if points are on different sides of plane
	if (d1 * d2 < 0.0f) 
	{ //clip to their intersection point? idk man
		float alpha = d1 / (d1 - d2);
		out[sp] = face->at(0) + alpha * (face->at(1) - face->at(0));
		++sp;
	}

	face->at(0) = out[0];
	face->at(1) = out[1];

	if (sp==3) { std::cout << "There two points behind the clipping plane and also they're on different sides? idk man what the hell is going on"; }

	return sp;
}

std::vector<float> Physics::FindAxisLeastPenetration(RigidBody* i_entA, RigidBody* i_entB)
{
	//These might not need to re recomputed
	std::vector<Vector2f> verticesA = i_entA->GetVertCords();
	std::vector<Vector2f> verticesB = i_entB->GetVertCords();
	std::vector<Vector2f> normalsA = i_entA->GetFaceNorms();
	float maxPenetrationDist = -std::numeric_limits<float>::max();
	float maxPenetrationVert = -1;
	for (int i = 0; i < verticesA.size(); ++i) {
		Vector2f bMaxVertProjAlongNormalA = GetMaxProjection(-normalsA[i], verticesB);
		//Not converting between object spaces, I really hope it wont break it to just do everything in world space for now
		//I THINK THIS IS FUCKING MY FPS TODO
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
	RigidBody* bodyA = &i_data->entA->rb;
	RigidBody* bodyB = &i_data->entB->rb;
	std::shared_ptr<Circle> circlePtrA = std::dynamic_pointer_cast<Circle>(bodyA->shape);
	std::shared_ptr<Circle> circlePtrB = std::dynamic_pointer_cast<Circle>(bodyB->shape);

	float combinedRad = (circlePtrA->radius + circlePtrB->radius);
	float radSqr = pow(combinedRad, 2);
	Vector2f collisionNorm = bodyB->transform.pos - bodyA->transform.pos;

	if (radSqr < collisionNorm.squaredNorm()) { return false; }

	/////////////////////////////////////////


	/////////YOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
	// I no longer know what this yo refers to, I should write more specific comments






	///////////////////////////////////////////////////
	float dist = sqrt(collisionNorm.squaredNorm());

	if (dist == 0.0f) {
		i_data->pen = circlePtrA->radius;
		i_data->norm = Vector2f(1.0f, 0.0f);
		i_data->contactPoints.push_back(bodyA->transform.pos);
		return true;
	}
	else {
		collisionNorm = collisionNorm / dist; //we already calculated sqrt
		i_data->norm = collisionNorm;
		i_data->pen = combinedRad - dist;
		i_data->contactPoints.push_back((collisionNorm * circlePtrA->radius) + bodyA->transform.pos);
		return true;
	}
}

bool Physics::ResolveRectToCircleCollision(CollisionData* i_data)
{  //Entity 1 is rectangle and entity 2 is circle
	RigidBody* rectBod = &i_data->entA->rb;
	RigidBody* circleBod = &i_data->entB->rb;
	std::shared_ptr<Circle> circlePtr = std::dynamic_pointer_cast<Circle>(circleBod->shape);

	float maxSeperation = -std::numeric_limits<float>::max();
	int collisionFaceIndex = -1;
	std::vector<Vector2f> rectVertices = rectBod->GetVertCords();
	std::vector<Vector2f> normals = rectBod->GetFaceNorms();
	for (int i = 0; i < normals.size(); ++i) {
		float currSeperation = normals[i].dot(circleBod->transform.pos - rectVertices[i]);
		if (currSeperation > circlePtr->radius) {
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
		i_data->norm = -(normals[collisionFaceIndex]);
		i_data->pen = circlePtr->radius;
		i_data->contactPoints.push_back((i_data->norm * circlePtr->radius) + circleBod->transform.pos);
		return true;
	}

	//Rest of code to determine veroni region 
	float dotProd1 = (circleBod->transform.pos - rectVert1).dot(rectVert2 - rectVert1);
	float dotProd2 = (circleBod->transform.pos - rectVert2).dot(rectVert1 - rectVert2);
	i_data->pen = circlePtr->radius - maxSeperation;

	// Circle center lies beyond face region, past V1
	if (dotProd1 <= 0.0f) {
		if ((circleBod->transform.pos - rectVert1).squaredNorm() > pow(circlePtr->radius, 2)) { return false; }
		Vector2f veroniNormal = rectVert1 - circleBod->transform.pos;
		veroniNormal.normalize();
		i_data->norm = veroniNormal;
		i_data->contactPoints.push_back(rectVert1 + rectBod->transform.pos);
	}
	// Circle center lies beyond face region, past V2
	else if (dotProd2 <= 0.0f) {
		if ((circleBod->transform.pos - rectVert2).squaredNorm() > pow(circlePtr->radius, 2)) { return false; }
		//ignorign some more stuff that I think is just translation to world space which Ive already done
		Vector2f veroniNormal = rectVert2 - circleBod->transform.pos;
		veroniNormal.normalize();
		i_data->norm = veroniNormal;
		i_data->contactPoints.push_back(rectVert2 + rectBod->transform.pos);
	}
	// Circle center is within face region
	else {
		Vector2f normal = normals[collisionFaceIndex];
		if ((circleBod->transform.pos - rectVert1).dot(normal) > circlePtr->radius) { return false; }
		//normal = -normal;
		i_data->norm = -normal;
		i_data->contactPoints.push_back((i_data->norm * circlePtr->radius) + circleBod->transform.pos);
	}
	return true;
}

bool Physics::ResolveRectToRectCollision(CollisionData* i_data) {
	std::vector<float> penetrationA = FindAxisLeastPenetration(&i_data->entA->rb, &i_data->entB->rb);
	if (penetrationA[0] > 0.0f) {
		return false;
	}
	std::vector<float> penetrationB = FindAxisLeastPenetration(&i_data->entB->rb, &i_data->entA->rb);
	if (penetrationB[0] > 0.0f) {
		return false;
	}

	int refIndex;
	bool flip;
	RigidBody* refEnt;
	RigidBody* incidentEnt;

	if (penetrationA[0] >= penetrationB[0]) {
		refEnt = &i_data->entA->rb;
		incidentEnt = &i_data->entB->rb;
		refIndex = penetrationA[1];
		flip = false;
	}
	else {
		refEnt = &i_data->entB->rb;
		incidentEnt = &i_data->entA->rb;
		refIndex = penetrationB[1];
		flip = true;
	}

	std::vector<Vector2f> incidentFaceVerts = FindIncidentFace(refEnt, incidentEnt, refIndex);
	std::vector<Vector2f> refVerts = refEnt->GetVertCords();

	Vector2f refEntV1 = refVerts[refIndex];
	Vector2f refEntV2 = refVerts[(refIndex + 1) % refVerts.size()];

	Vector2f refSidePlaneNormal = refEntV2 - refEntV1;
	refSidePlaneNormal.normalize();

	Vector2f refFaceNormal = refSidePlaneNormal.unitOrthogonal();

	float refC = refFaceNormal.dot(refEntV1);
	float refNegSide = -refSidePlaneNormal.dot(refEntV1);
	float refPosSide = refSidePlaneNormal.dot(refEntV2);

	if (Clip(-refSidePlaneNormal, refNegSide, &incidentFaceVerts) < 2) { return false; }
	if (Clip(refSidePlaneNormal, refPosSide, &incidentFaceVerts) < 2) { return false; }

	i_data->norm = flip ? -refFaceNormal : refFaceNormal;

	float seperation = refFaceNormal.dot(incidentFaceVerts[0]) - refC;

	int contactPoints = 0;
	if (seperation <= 0.0f) {
		i_data->contactPoints.push_back(incidentFaceVerts[0]);
		i_data->pen = -seperation;
		++contactPoints;
	}
	else {
		i_data->pen = 0;
	}
	//why is this checking the same shit
	seperation = refFaceNormal.dot(incidentFaceVerts[1]) - refC;
	if (seperation <= 0.0f) {
		i_data->contactPoints.push_back(incidentFaceVerts[1]);
		++contactPoints;
		i_data->pen -= seperation;
		i_data->pen /= contactPoints;
	}

	return true;
}

std::function<void(void)> Physics::SaveImpulse(RigidBody* body, Vector2f a1, Vector2f a2)
{
	return [=]() { body->ApplyImpulse(a1, a2); }; // Capture arguments by value, except body as pointer
}

bool Physics::CheckCollision(CollisionData* i_collision)
{
	Shape::ShapeType shape1 = i_collision->entA->rb.shape->GetType();
	Shape::ShapeType shape2 = i_collision->entB->rb.shape->GetType();
	bool collisionOccured = false;
	if ((shape1 == Shape::CIRCLE) && (shape2 == Shape::CIRCLE))
	{
		collisionOccured = ResolveCircleToCircleCollision(i_collision);
	}
	else if (((shape1 == Shape::RECTANGLE) || (shape1 == Shape::POLYGON)) && ((shape2 == Shape::RECTANGLE) || (shape2 == Shape::POLYGON)))
	{
		collisionOccured = ResolveRectToRectCollision(i_collision);
	}
	else if (((shape1 == Shape::RECTANGLE)||(shape1 == Shape::POLYGON)) && (shape2 == Shape::CIRCLE))
	{
		collisionOccured = ResolveRectToCircleCollision(i_collision);
		std::shared_ptr<Entity> temp = i_collision->entA;
		i_collision->entA = i_collision->entB;
		i_collision->entB = temp;
	}
	else if ((shape1 == Shape::CIRCLE) && ((shape2 == Shape::RECTANGLE)||(shape2 == Shape::POLYGON)))
	{
		std::shared_ptr<Entity> temp = i_collision->entA;
		i_collision->entA = i_collision->entB;
		i_collision->entB = temp;
		collisionOccured = ResolveRectToCircleCollision(i_collision);
		temp = i_collision->entA;
		i_collision->entA = i_collision->entB;
		i_collision->entB = temp;
	}
	return collisionOccured;
}

void Physics::InfiniteMassCorrection(CollisionData * i_data)
{
	i_data->entA->rb.vel = Vector2f(0.0f, 0.0f);
	i_data->entB->rb.vel = Vector2f(0.0f, 0.0f);
}

void Physics::PositionalCorrection(CollisionData * i_data)
{
	if (i_data->entA->intangible || i_data->entB->intangible) { // these items have no impulse upon collision
		return;
	}

	if ((i_data->entA->rb.massD.GetMass() + i_data->entB->rb.massD.GetMass()) == 0.0f) {
		//Vector2f correction = std::max(i_data->pen - PENETRATION_ALLOWANCE, 0.0f) / 0.5f * i_data->norm * PENETRATION_CORRECTION;
		//i_data->bodyA->transform.pos = i_data->bodyA->transform.pos - (0.5f * correction);
		//i_data->bodyB->transform.pos = i_data->bodyB->transform.pos + (0.5f * correction);
		//Not gonna do anything to correct infinite mass objects touching each other
		return;
	}
	else {
		Vector2f correction = std::max(i_data->pen - PENETRATION_ALLOWANCE, 0.0f) /
			(i_data->entA->rb.massD.GetMassInv() + i_data->entB->rb.massD.GetMassInv()) * i_data->norm * PENETRATION_CORRECTION;

		i_data->entA->rb.AdjustPosition(-correction * i_data->entA->rb.massD.GetMassInv());
		i_data->entB->rb.AdjustPosition(correction * i_data->entB->rb.massD.GetMassInv());
	}
}

std::shared_ptr<Polygon> Physics::CreateRegularPolygon(int i_numVerts, float i_size)
{
	std::vector<Vector2f> vertVect;
	for (int i = 0; i < i_numVerts; ++i) {
		float radAngle = ((((float)(i_numVerts-i)/(float)i_numVerts))*2.0f*PI);
		Vector2f vert = Math::AngleToVect(radAngle) * i_size;
		vertVect.push_back(vert);
	}
	return std::make_shared<Polygon>(vertVect);
}

std::shared_ptr<Polygon> Physics::CreateIrregularPolygon(int i_numVerts, float i_size)
{ //GOnna use an algorithm I found here in Java : http://cglab.ca/~sander/misc/ConvexGeneration/convex.html

	std::random_device rd1;
	std::mt19937 gen1(rd1());
	std::uniform_int_distribution<> distrib(1, i_size); //both boundaries are inclusive

	std::vector<float> xPool = {};
	std::vector<float> yPool = {};

	for (int i = 0; i < i_numVerts; i++) {
		xPool.push_back(distrib(gen1));
		yPool.push_back(distrib(gen1));
		}

	std::sort(xPool.begin(), xPool.begin() + xPool.size());
	std::sort(yPool.begin(), yPool.begin() + yPool.size());
	float minX = xPool[0];
	float maxX = xPool[xPool.size()-1];
	float minY = yPool[0];
	float maxY = yPool[xPool.size() - 1];
	
	std::vector<float> xVec = {};
	std::vector<float> yVec = {};
	
	float lastTop = minX;
	float lastBot = minX;
	
	for (int i = 1; i < i_numVerts - 1; i++) {
		float x = xPool[i];
		if (distrib(gen1) % 2 == 1) {
			xVec.push_back(x - lastTop);
			lastTop = x;
		}
		else {
			xVec.push_back(lastBot - x);
			lastBot = x;
		}
	}

	xVec.push_back(maxX - lastTop);
	xVec.push_back(lastBot - maxX);
	
	float lastLeft = minY;
	float lastRight = maxY;

	for (int i = 1; i < i_numVerts; i++) {
		float y = yPool[i];
		if (distrib(gen1) % 2 == 1) {
			yVec.push_back(y - lastLeft);
			lastLeft = y;
		}
		else {
			yVec.push_back(lastRight - y);
			lastRight = y;
		}
	}

	yVec.push_back(maxY - lastLeft);
	yVec.push_back(lastRight - maxY);

	//https://en.cppreference.com/w/cpp/algorithm/random_shuffle
	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(yVec.begin(), yVec.end(), g);

	std::vector<Vector2f> vectorVect = {};

	for (int i = 0; i < i_numVerts; i++) {
		vectorVect.push_back(Vector2f(xVec[i], yVec[i]));
	}

	std::sort(vectorVect.begin(), vectorVect.end(), [](const Vector2f & v1, const Vector2f & v2) {
		return atan2(v1.y(), v1.x()) > atan2(v2.y(), v2.x());
	}); // I DONT KNOW IF IM SUPPOSED TO BE SORTING LOW TO HIGH OR HIGH TO LOW, ASSUMING LOW TO HIGH
	
	float x = 0;
	float y = 0;
	float minPolyX = 0;
	float minPolyY = 0;
	std::vector<Vector2f> points = {};

	for (int i = 0; i < i_numVerts; i++) {
		points.push_back(Vector2f(x, y));

		x += vectorVect[i].x();
		y += vectorVect[i].y();

		minPolyX = std::min(minPolyX, x);
		minPolyY = std::min(minPolyY, y);
	}

	float xShift = minX - minPolyX;
	float yShift = minY - minPolyY;

	for (int i = 0; i < i_numVerts; i++) {
		Vector2f vert = points[i];
		points[i] = Vector2f(vert.x() + xShift, vert.y() + yShift);
	}

	return std::make_shared<Polygon>(points);

	std::vector<Vector2f> vertVect;
	////for (int i = 0; i < i_numVerts; ++i) {
	////	float radAngle = ((((float)(i_numVerts - i) / (float)i_numVerts))*2.0f*PI);
	////	Vector2f vert = Math::AngleToVect(radAngle) * i_size;
	////	vertVect.push_back(vert);
	////}
	////
}

void Physics::CreateCollisionImpulse(CollisionData* i_data) {
	RigidBody* bodyA = &i_data->entA->rb;
	RigidBody* bodyB = &i_data->entB->rb;
	float statFric = bodyA->mat.statFrict * bodyB->mat.statFrict;
	float dynaFric = bodyA->mat.dynaFrict * bodyB->mat.dynaFrict;

	if (i_data->entA->intangible || i_data->entB->intangible) { // these items have no impulse upon collision
		return;
	}

	if (!bodyA->massD.GetMassInv() && !bodyB->massD.GetMassInv()) {
		InfiniteMassCorrection(i_data);
		return;
	}

	std::vector<std::function<void(void)>> impulseCalls;

	for (Vector2f contactPoint : i_data->contactPoints) {

		Vector2f contactPtA = contactPoint - bodyA->transform.pos;
		Vector2f contactPtB = contactPoint - bodyB->transform.pos;
		Vector2f bodyBAngVelAtContactP = Math::FloatVectCross(bodyB->GetInstAngVel(), contactPtB);
		Vector2f bodyAAngVelAtContactP =Math::FloatVectCross(bodyA->GetInstAngVel(), contactPtA);
		Vector2f relativeVelocity = (bodyB->GetInstVel() + bodyBAngVelAtContactP) - (bodyA->GetInstVel() - bodyAAngVelAtContactP);

		float  contactRelVel = relativeVelocity.dot(i_data->norm);
		if (contactRelVel > 0) { 
			continue; 
		}

		float crossA = Math::CrossProdScalar(contactPtA, i_data->norm);
		float crossB = Math::CrossProdScalar(contactPtB, i_data->norm);

		float invMassSum = bodyA->massD.GetMassInv() + bodyB->massD.GetMassInv() +
						   (pow(crossA, 2) * bodyA->massD.GetInertInv()) +
						   (pow(crossB, 2) * bodyB->massD.GetInertInv());

		float impulseScalar = -(1.0f + std::max(bodyA->mat.rest, bodyB->mat.rest)) * contactRelVel;
		impulseScalar /= invMassSum; //this seems to be too high
		impulseScalar /= i_data->contactPoints.size();

		Vector2f impulse = i_data->norm * impulseScalar;

		impulseCalls.push_back(SaveImpulse(bodyA, -impulse, contactPtA));
		impulseCalls.push_back(SaveImpulse(bodyB, impulse, contactPtB));
		//bodyA->ApplyImpulse(-impulse, contactPtA);
		//bodyB->ApplyImpulse(impulse, contactPtB);

		relativeVelocity = (bodyB->GetInstVel() + Math::FloatVectCross(bodyB->GetInstAngVel(), contactPtB)) -
						   (bodyA->GetInstVel() - Math::FloatVectCross(bodyA->GetInstAngVel(), contactPtA));

		Vector2f tan = relativeVelocity - (i_data->norm * relativeVelocity.dot(i_data->norm));
		tan.normalize();
		float tanMag = -relativeVelocity.dot(tan);
		tanMag /= invMassSum;
		tanMag /= i_data->contactPoints.size();

		//dont apply tiny friction impulses or else stuff will just bounce around forever
		if (abs(tanMag) < 0.05f) { 
			continue;
		}

		//Coulumbs law:
		Vector2f tangentImpulse;
		if (abs(tanMag) < impulseScalar * statFric) { tangentImpulse = tan * tanMag; }
		else { tangentImpulse = tan * -impulseScalar * dynaFric; }

		if (bodyA->shape->GetType() == Shape::CIRCLE) {
			impulseCalls.push_back(SaveImpulse(bodyA, tangentImpulse, contactPtA));
			//bodyA->ApplyImpulse(tangentImpulse, contactPtA);
		}
		else {
			impulseCalls.push_back(SaveImpulse(bodyA, -tangentImpulse, contactPtA));
			//bodyA->ApplyImpulse(-tangentImpulse, contactPtA);
		}
		if (bodyB->shape->GetType() == Shape::CIRCLE) {
			impulseCalls.push_back(SaveImpulse(bodyB, -tangentImpulse, contactPtB));
			//bodyB->ApplyImpulse(-tangentImpulse, contactPtB);
		}
		else {
			impulseCalls.push_back(SaveImpulse(bodyB, tangentImpulse, contactPtB));
			//bodyB->ApplyImpulse(-tangentImpulse, contactPtB);
		}
	}

	for (std::function<void(void)> impulseCall : impulseCalls) {
		impulseCall();
	}

	return;
}
