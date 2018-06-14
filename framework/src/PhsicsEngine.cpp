#include "Header/PhysicsEngine.h"

PhysicsEngine::PhysicsEngine() {
    velocity = glm::vec3(0.f, 0.f, 0.f);
    gravity = glm::vec3(0.f, GravityAcceler, 0.f);
    accelerUp = glm::vec3(0.f, 0.f, 0.f);

    isJumping = false;
}

PhysicsEngine::~PhysicsEngine() {
}

void PhysicsEngine::setSceneOuterBoundary(float x1, float z1, float x2, float z2) {
    outerBoundary = glm::vec4(x1, z1, x2, z2);
}

void PhysicsEngine::setSceneInnerBoundary(float x1, float y1, float z1, float x2, float y2, float z2) {
    glm::vec3 key(x1 - BoundaryGap, y1 - BoundaryGap, z1 - BoundaryGap);
    glm::vec3 value(x2 + BoundaryGap, y2 + BoundaryGap, z2 + BoundaryGap);

    innerBoundaryMin.push_back(key);
    innerBoundaryMax.push_back(value);
}

void PhysicsEngine::outCollisionTest(glm::vec3 & cameraPos, glm::vec3 & targetPos) {
    outCollisionTestXZ(outerBoundary[0], outerBoundary[1], outerBoundary[2], outerBoundary[3], cameraPos, targetPos);
}

void PhysicsEngine::outCollisionTestXZ(float x1, float z1, float x2, float z2, glm::vec3 & cameraPos, glm::vec3 & targetPos) {
	//先设置包围盒：比空间外部边缘小一点	
    x1 += 2;z1 += 2;
    x2 -= 2;z2 -= 2;
    glm::vec3 delta = 0.11f * (targetPos - cameraPos);

	//检测视点目标是否出了包围盒，若是则放回
	if (targetPos[0] < x1 || targetPos[0] > x2 || targetPos[2] < z1 || targetPos[2] > z2) {
		targetPos-=delta;
        cameraPos-=delta;
	}
}

void PhysicsEngine::jumpAndUpdateVelocity() {
	velocity = glm::vec3(0.f, JumpInitialSpeed, 0.f);
	accelerUp.y = 0.f;
}

//判断在xz平面，相机位置是否位于碰撞体内部
bool insideTheCollider(glm::vec3 _cameraPos, glm::vec3 _innerMin, glm::vec3 _innerMax) {
	float camX = _cameraPos.x;
	float camZ = _cameraPos.z;
	float minX = _innerMin.x;
	float minZ = _innerMin.z;
	float maxX = _innerMax.x;
	float maxZ = _innerMax.z;

	if (minX <= camX && camX <= maxX && minZ <= camZ && camZ <= maxZ)
		return true;
	else
		return false;
}

void PhysicsEngine::updateCameraVertMovement(glm::vec3 & cameraPos, glm::vec3 & targetPos,GLfloat deltaTime) 
{
	glm::vec3 acceleration = gravity + accelerUp;
	velocity += acceleration * GravityFactor;
	cameraPos += velocity * JumpFactor * deltaTime;
	targetPos += velocity * JumpFactor * deltaTime;

	//检测所有碰撞体
	for (unsigned int i = 0; i < innerBoundaryMin.size(); i++) {
		//如果在XZ平面进入碰撞体所在区域
		if (insideTheCollider(cameraPos, innerBoundaryMin[i], innerBoundaryMax[i])) {
			if (cameraPos.y - HeroHeight <= innerBoundaryMax[i][1]
				&& cameraPos.y >= innerBoundaryMax[i][1]) {              //脚接触到碰撞体顶部
				//cout << "touch the top of collider" << endl;
				isJumping = false;
				accelerUp.y = -GravityAcceler;
				velocity.y = 0.f;
				cameraPos.y = innerBoundaryMax[i][1] + HeroHeight;
				break;
			}

			if (cameraPos.y >= innerBoundaryMin[i][1] &&
				cameraPos.y - HeroHeight <= innerBoundaryMin[i][1]) {    //头接触到碰撞体底部
				//cout << "touch the bottom of collider" << endl;
				velocity.y = 0.f;
				cameraPos.y = innerBoundaryMin[i][1];
				break;
			}
		}
		else {
			accelerUp.y = 0.f;
		}
	}
}

void PhysicsEngine::inCollisionTest(glm::vec3 & cameraPos, glm::vec3 & targetPos) {
	//后面可以在这里添加：预处理，排除当前肯定不会产生碰撞的物体
	for (unsigned int i = 0; i < innerBoundaryMin.size(); i++) {
		inCollisionTestWithHeight(innerBoundaryMin[i][0], innerBoundaryMin[i][1], innerBoundaryMin[i][2],
			innerBoundaryMax[i][0], innerBoundaryMax[i][1], innerBoundaryMax[i][2], cameraPos, targetPos);
	}
}

void PhysicsEngine::inCollisionTestWithHeight(float x1, float y1, float z1, float x2, float y2, float z2, glm::vec3 & cameraPos, glm::vec3 & targetPos) {
	//当身体处于碰撞体垂直区域范围内，才进行XZ平面的碰撞检测
	if (!(cameraPos[1] <= y1 || cameraPos[1] - HeroHeight >= y2)) {
		inCollisionTestXZ(x1, z1, x2, z2, cameraPos, targetPos);
	}
}
double Direction(dot pi, dot pj, dot pk) {
	return (pk.x - pi.x)*(pj.y - pi.y) - (pj.x - pi.x)*(pk.y - pi.y);
}

bool OnSegment(dot pi, dot pj, dot pk) {
	if ((_min(pi.x, pj.x) <= pk.x) && (pk.x <= _max(pi.x, pj.x))
		&& (_min(pi.y, pj.y) <= pk.y) && (pk.y <= _max(pi.y, pj.y)))
		return true;
	else return false;
}

//检测线段相交快速算法
bool SegmentIntersect(dot p1, dot p2, dot p3, dot p4) {
	double d1, d2, d3, d4;
    const double gap=0.01;
	d1 = Direction(p3, p4, p1);
	d2 = Direction(p3, p4, p2);
	d3 = Direction(p1, p2, p3);
	d4 = Direction(p1, p2, p4);
	if (((d1 > 0.0 && d2 < 0.0) || (d1 < 0.0 && d2> 0.0)) && ((d3> 0.0 && d4 < 0.0) || (d3 < 0.0 && d4> 0.0)))
		return true;
	else if (d1 < gap && d1 > -gap && OnSegment(p3, p4, p1))
		return true;
	else if (d2 < gap && d2 > -gap && OnSegment(p3, p4, p2))
		return true;
	else if (d3 < gap && d3 > -gap && OnSegment(p1, p2, p3))
		return true;
	else if (d3 < gap && d3 > -gap && OnSegment(p1, p2, p4))
		return true;
	else
		return false;
}

void PhysicsEngine::inCollisionTestXZ(float x1, float z1, float x2, float z2, glm::vec3 & cameraPos, glm::vec3 & targetPos) {
	//float tarX = targetPos[0], camX = cameraPos[0], tarZ = targetPos[2], camZ = cameraPos[2];
	//float len = sqrt((camX - tarX)*(camX - tarX) + (camZ - tarZ)*(camZ - tarZ));
    glm::vec3 delta = 0.11f*(targetPos - cameraPos);

	dot d1(cameraPos[0], cameraPos[2]), d2(targetPos[0], targetPos[2]);
	dot d3(x1, z1), d4(x1, z2), d5(x2, z1), d6(x2, z2);
    // printf("cameraPos:%f %f, targetPos:%f %f\n",d1.x,d1.y,d2.x,d2.y);
    // printf("d4: %f %f, d6: %f %f\n",d4.x,d4.y,d6.x,d6.y);
    // printf("%d\n",SegmentIntersect(d1, d2, d4, d6));
	if (SegmentIntersect(d1, d2, d4, d6)) {
		if (targetPos[2] < cameraPos[2]) {
			// printf("1\n");
			//仅改变z坐标
            targetPos-=glm::vec3(0.0f,0.0f,delta.z);
            cameraPos-=glm::vec3(0.0f,0.0f,delta.z);
		}
		else if (targetPos[2] > cameraPos[2]) {
			// printf("2\n");
            targetPos-=glm::vec3(0.0f,0.0f,delta.z);
            cameraPos-=glm::vec3(0.0f,0.0f,delta.z);
		}
	}
	else if (SegmentIntersect(d1, d2, d5, d6)) {
		if (targetPos[0]<cameraPos[0]) {
			// printf("3\n");
            targetPos-=glm::vec3(delta.x,0.0f,0.0f);
            cameraPos-=glm::vec3(delta.x,0.0f,0.0f);
		}
		else if (targetPos[0]>cameraPos[0]) {
			// printf("4\n");
            targetPos-=glm::vec3(delta.x,0.0f,0.0f);
            cameraPos-=glm::vec3(delta.x,0.0f,0.0f);
		}
	}
	else if (SegmentIntersect(d1, d2, d3, d5)) {
		if (targetPos[2] > cameraPos[2]) {
			// printf("5\n");
            targetPos-=glm::vec3(0.0f,0.0f,delta.z);
            cameraPos-=glm::vec3(0.0f,0.0f,delta.z);
		}
		else if (targetPos[2] < cameraPos[2]) {
			// printf("6\n");
            targetPos-=glm::vec3(0.0f,0.0f,delta.z);
            cameraPos-=glm::vec3(0.0f,0.0f,delta.z);
		}
	}
	else if (SegmentIntersect(d1, d2, d3, d4)) {
		if (targetPos[0] > cameraPos[0]) {
			// printf("7\n");
            targetPos-=glm::vec3(delta.x,0.0f,0.0f);
            cameraPos-=glm::vec3(delta.x,0.0f,0.0f);
		}
		else if (targetPos[0] < cameraPos[0]) {
			// printf("8\n");
            targetPos-=glm::vec3(delta.x,0.0f,0.0f);
            cameraPos-=glm::vec3(delta.x,0.0f,0.0f);
		}
	}
}