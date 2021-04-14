#include "ofApp.h"	

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofSetLineWidth(1.5);
	ofEnableDepthTest();

	this->frame.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	this->face.clear();
	this->frame.clear();

	float R = 300;
	this->r = ofMap(ofNoise(ofRandom(1000), ofGetFrameNum() * 0.008), 0, 1, 10, 130);
	int v_span = 8;
	int u_span = 10;

	for (int v = 0; v < 360; v += v_span) {

		for (auto u = 0; u < 360; u += u_span) {

			auto noise_value = ofNoise(glm::vec4(this->make_point(R, 50, u + u_span * 0.5, v) * 0.005, ofGetFrameNum() * 0.01));

			if (noise_value > 0.6) {

				vector<glm::vec3> vertices;
				vertices.push_back(this->make_point(R, this->r, u, v - v_span * 0.5));
				vertices.push_back(this->make_point(R, this->r, u + u_span, v - v_span * 0.5));
				vertices.push_back(this->make_point(R, this->r, u + u_span, v + v_span * 0.5));
				vertices.push_back(this->make_point(R, this->r, u, v + v_span * 0.5));

				int index = face.getNumVertices();
				this->face.addVertices(vertices);

				this->face.addIndex(index + 0); face.addIndex(index + 1); face.addIndex(index + 2);
				this->face.addIndex(index + 0); face.addIndex(index + 2); face.addIndex(index + 3);

				this->frame.addVertices(vertices);

				this->frame.addIndex(index + 0); this->frame.addIndex(index + 1);
				this->frame.addIndex(index + 1); this->frame.addIndex(index + 2);
				this->frame.addIndex(index + 2); this->frame.addIndex(index + 3);
				this->frame.addIndex(index + 3); this->frame.addIndex(index + 0);
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();

	int number = 300;
	vector<glm::vec3> location_list(number);
	for (int i = 0; i < number; i++) {

		auto R = 300;
		auto tmp_r = ofRandom(10, this->r * 0.8);
		auto u = ofRandom(360) + ofGetFrameNum() * ofRandom(0.5, 1.0) * (ofRandom(1) < 0.5 ? 1 : -1);
		auto v = ofRandom(360) + ofGetFrameNum() * ofRandom(0.5, 1.0) * (ofRandom(1) < 0.5 ? 1 : -1);

		location_list[i] = this->make_point(R, tmp_r, u, v);
	}

	ofFill();
	ofSetColor(39);
	for (auto& location : location_list) {

		ofDrawSphere(location, 3);
		for (auto& other : location_list) {

			if (location == other) { continue; }

			auto distance = glm::distance(location, other);
			if (distance < 40) {

				ofDrawLine(location, other);
			}
		}
	}

	ofSetColor(239);
	this->face.draw();

	ofSetColor(39);
	this->frame.drawWireframe();

	this->cam.end();
}

//--------------------------------------------------------------
glm::vec3 ofApp::make_point(float R, float r, float u, float v) {

	// 数学デッサン教室 描いて楽しむ数学たち　P.31

	u *= DEG_TO_RAD;
	v *= DEG_TO_RAD;

	auto x = (R + r * cos(u)) * cos(v);
	auto y = (R + r * cos(u)) * sin(v);
	auto z = r * sin(u);

	return glm::vec3(x, y, z);
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}