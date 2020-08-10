#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
class Transform
{
public:
	Transform(const glm::vec3& position = glm::vec3(), const glm::vec3& rotation = glm::vec3(), const glm::vec3& scale = glm::vec3(1.0f, 1.0f, 1.0f)) : //Definition for Transform function that takes in 3 const glm::vec3's as arguments, 2 of these arguments are defaulted to null and scale is defaulted to 1.0 for each coordinate
		_position(position), //initializes position
		_rotation(rotation), //initializes rotation 
		_scale(scale) {} //initializes scale

	inline glm::mat4 GetModel() const
	{
		glm::mat4 positionMatrix = glm::translate(_position); //generates 4x4 matrix containing the position
		glm::mat4 rotationXMatrix = glm::rotate(_rotation.x, glm::vec3(1, 0, 0)); //generates 4x4 matrix for the x rotation
		glm::mat4 rotationYMatrix = glm::rotate(_rotation.y, glm::vec3(0, 1, 0)); //generates 4x4 matrix for the y rotation
		glm::mat4 rotationZMatrix = glm::rotate(_rotation.z, glm::vec3(0, 0, 1)); //generates 4x4 matrix for the z rotation
		glm::mat4 scaleMatrix = glm::scale(_scale); //generates 4x4 matrix containing the scale
		glm::mat4 rotationMatrix = rotationZMatrix * rotationYMatrix * rotationXMatrix; //calculate rotation matrix
		return positionMatrix * rotationMatrix * scaleMatrix; //return final rotation matrix 
	}
	inline glm::vec3& GetPostition() { return _position; } //gets position
	inline glm::vec3& GetRotation() { return _rotation; } //gets rotation
	inline glm::vec3& GetScale() { return _scale; } //gets scale

	inline void SetPosition(const glm::vec3& position) { _position = position; } //sets position
	inline void SetRotation(const glm::vec3& rotation) { _rotation = rotation; } //sets rotation
	inline void SetScale(const glm::vec3& scale) { _scale = scale; } //sets scale

protected:
private:
	glm::vec3 _position; //creates variable of type glm::vec3 named _position 
	glm::vec3 _rotation; //creates variable of type glm::vec3 named _rotation 
	glm::vec3 _scale; //creates variable of type glm::vec3 named _scale

};