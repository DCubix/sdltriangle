#include <iostream>
#include <string>
#include <cstdint>
#include <cstring>
#include <algorithm>
#include <memory>
#include <cmath>
#include <array>

#if __has_include("SDL2/SDL.h")
#	include "SDL2/SDL.h"
#else
#	include "SDL.h"
#endif

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "vec_math.hpp"

#define lerp(a, b, t) ((1.0f - (t)) * (a) + (b) * (t))
#define ntob(n) uint8_t((n) * 255.0f)

struct RGB {
	float r, g, b;

	RGB() = default;
	RGB(float r, float g, float b) : r(r), g(g), b(b) {}

	RGB mix(RGB with, float t) {
		return RGB(lerp(r, with.r, t), lerp(g, with.g, t), lerp(b, with.b, t));
	}

	void gammaCorrect(float ratio = 1.0f / 2.2f) {
		r = std::pow(r, ratio);
		g = std::pow(g, ratio);
		b = std::pow(b, ratio);
	}
};

struct Vertex {
	int x, y;
	RGB color;
	float s, t;

	Vertex() = default;
	Vertex(int x, int y, RGB color, float s = 0.0f, float t = 0.0f)
		: x(x), y(y), color(color), s(s), t(t)
	{}
};

class Texture {
public:
	Texture() = default;
	Texture(const std::string& fileName) {
		int comp;
		m_pixels = stbi_load(fileName.c_str(), &m_width, &m_height, &comp, 3);
	}

	~Texture() {
		stbi_image_free(m_pixels);
	}

	RGB get(int x, int y) {
		x = std::abs(x) % (m_width-1);
		y = std::abs(y) % (m_height-1);
		const int i = (x + y * m_width) * 3;
		RGB col{
			float(m_pixels[i + 0]) / 255.0f,
			float(m_pixels[i + 1]) / 255.0f,
			float(m_pixels[i + 2]) / 255.0f
		};
		return col;
	}

	int width() const { return m_width; }
	int height() const { return m_height; }

private:
	int m_width, m_height;
	uint8_t* m_pixels;
};

class Renderer {
public:
	struct Camera {
		Vector3 position{};
		float angle{ 0.0f }, pitch{ 5.5f }, zoom{ 1.0f };
	};

	Renderer() = default;

	Renderer(SDL_Window* window, SDL_Renderer* renderer, int pixelSize = 2)
		: m_renderer(renderer), m_window(window)
	{
		int w, h;
		SDL_GetWindowSize(window, &w, &h);
		m_windowWidth = w;
		m_windowHeight = h;
		m_bufferWidth = w / pixelSize;
		m_bufferHeight = h / pixelSize;
		m_buffer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, m_bufferWidth, m_bufferHeight);

		int p;
		SDL_LockTexture(m_buffer, nullptr, (void**) &m_pixels, &p);
	}

	virtual ~Renderer() {
		SDL_DestroyTexture(m_buffer);
	}

	void clear(RGB color) {
		for (int y = 0; y < m_bufferHeight; y++) {
			for (int x = 0; x < m_bufferWidth; x++) {
				dot(x, y, color);
			}
		}
	}

	void dot(int x, int y, RGB color) {
		if (x < 0 || x >= m_bufferWidth || y < 0 || y >= m_bufferHeight) return;
		const int i = (x + y * m_bufferWidth) * 3;
		//color.gammaCorrect();
		m_pixels[i + 0] = ntob(color.r);
		m_pixels[i + 1] = ntob(color.g);
		m_pixels[i + 2] = ntob(color.b);//std::pow((n), 1.0f/2.2f)
	}

	float edgeF(const Vector2& a, const Vector2& b, const Vector2& c) {
		return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
	}

	void triangle(Vertex v1, Vertex v2, Vertex v3) {
		int minX = std::min({v1.x, v2.x, v3.x});
		int maxX = std::max({v1.x, v2.x, v3.x});
		int minY = std::min({v1.y, v2.y, v3.y});
		int maxY = std::max({v1.y, v2.y, v3.y});

		Vector2 p1{ float(v1.x), float(v1.y) };
		Vector2 p2{ float(v2.x), float(v2.y) };
		Vector2 p3{ float(v3.x), float(v3.y) };
		float k = edgeF(p1, p2, p3);

		for (int y = minY; y <= maxY; y++) {
			for (int x = minX; x <= maxX; x++) {
				Vector2 p{ float(x), float(y) };
				float w1 = edgeF(p2, p3, p),
					  w2 = edgeF(p3, p1, p),
					  w3 = edgeF(p1, p2, p);

				if (w1 >= 0.0f && w2 >= 0.0f && w3 >= 0.0f) {
					w1 /= k;
					w2 /= k;
					w3 /= k;

					RGB col;
					col.r = w1 * v1.color.r + w2 * v2.color.r + w3 * v3.color.r;
					col.g = w1 * v1.color.g + w2 * v2.color.g + w3 * v3.color.g;
					col.b = w1 * v1.color.b + w2 * v2.color.b + w3 * v3.color.b;

					if (m_texture != nullptr) {
						float s = w1 * v1.s + w2 * v2.s + w3 * v3.s;
						float t = w1 * v1.t + w2 * v2.t + w3 * v3.t;
						RGB tex = m_texture->get(std::floor(s * m_texture->width() + 0.5f), std::floor(t * m_texture->height() + 0.5f));
						col.r *= tex.r;
						col.g *= tex.g;
						col.b *= tex.b;
					}
					dot(x, y, col);
				}
			}
		}
	}

	void quad(Vertex v1, Vertex v2, Vertex v3, Vertex v4) {
		triangle(v3, v2, v1);
		triangle(v1, v4, v3);
	}

	void cube(const Vector2& position, float angle = 0.0f, float pitch = 0.0f, float scale = 64.0f) {
		pitch -= m_camera.pitch;
		angle -= m_camera.angle;
		scale *= m_camera.zoom;

		std::array<Vector3, 8> unitCube, rotCube, worldCube, projCube;
		unitCube[0] = { 0.0f, 0.0f, 0.0f };
		unitCube[1] = { scale, 0.0f, 0.0f };
		unitCube[2] = { scale, scale, 0.0f };
		unitCube[3] = { 0.0f, scale, 0.0f };
		unitCube[4] = { 0.0f, 0.0f, scale };
		unitCube[5] = { scale, 0.0f, scale };
		unitCube[6] = { scale, scale, scale };
		unitCube[7] = { 0.0f, scale, scale };

		// XZ plane translation
		for (int i = 0; i < 8; i++) {
			unitCube[i].x += position.x * scale - m_camera.position.x;
			unitCube[i].y += -m_camera.position.y;
			unitCube[i].z += position.y * scale - m_camera.position.z;
		}

		// Rotate Y
		float c = std::cos(angle);
		float s = std::sin(angle);
		for (int i = 0; i < 8; i++) {
			rotCube[i].x = unitCube[i].x * c + unitCube[i].z * s;
			rotCube[i].y = unitCube[i].y;
			rotCube[i].z = unitCube[i].x * -s + unitCube[i].z * c;
		}

		// Rotate X
		c = std::cos(pitch);
		s = std::sin(pitch);
		for (int i = 0; i < 8; i++) {
			worldCube[i].x = rotCube[i].x;
			worldCube[i].y = rotCube[i].y * c - rotCube[i].z * s;
			worldCube[i].z = rotCube[i].y * s + rotCube[i].z * c;
		}

		// Ortho projection
		for (int i = 0; i < 8; i++) {
			projCube[i].x = worldCube[i].x + m_bufferWidth * 0.5f;
			projCube[i].y = worldCube[i].y + m_bufferHeight * 0.5f;
			projCube[i].z = worldCube[i].z;
		}

		quad(
			Vertex(projCube[7].x, projCube[7].y, RGB(1.0f, 1.0f, 1.0f), 0.0f, 0.0f),
			Vertex(projCube[6].x, projCube[6].y, RGB(1.0f, 1.0f, 1.0f), 1.0f, 0.0f),
			Vertex(projCube[2].x, projCube[2].y, RGB(1.0f, 1.0f, 1.0f), 1.0f, 1.0f),
			Vertex(projCube[3].x, projCube[3].y, RGB(1.0f, 1.0f, 1.0f), 0.0f, 1.0f)
		);

		quad(
			Vertex(projCube[0].x, projCube[0].y, RGB(1.0f, 1.0f, 1.0f), 0.0f, 0.0f),
			Vertex(projCube[1].x, projCube[1].y, RGB(1.0f, 1.0f, 1.0f), 1.0f, 0.0f),
			Vertex(projCube[2].x, projCube[2].y, RGB(1.0f, 1.0f, 1.0f), 1.0f, 1.0f),
			Vertex(projCube[3].x, projCube[3].y, RGB(1.0f, 1.0f, 1.0f), 0.0f, 1.0f)
		);

		quad(
			Vertex(projCube[7].x, projCube[7].y, RGB(1.0f, 1.0f, 1.0f), 0.0f, 0.0f),
			Vertex(projCube[6].x, projCube[6].y, RGB(1.0f, 1.0f, 1.0f), 1.0f, 0.0f),
			Vertex(projCube[5].x, projCube[5].y, RGB(1.0f, 1.0f, 1.0f), 1.0f, 1.0f),
			Vertex(projCube[4].x, projCube[4].y, RGB(1.0f, 1.0f, 1.0f), 0.0f, 1.0f)
		);

		quad(
			Vertex(projCube[3].x, projCube[3].y, RGB(1.0f, 1.0f, 1.0f), 0.0f, 0.0f),
			Vertex(projCube[7].x, projCube[7].y, RGB(1.0f, 1.0f, 1.0f), 1.0f, 0.0f),
			Vertex(projCube[4].x, projCube[4].y, RGB(1.0f, 1.0f, 1.0f), 1.0f, 1.0f),
			Vertex(projCube[0].x, projCube[0].y, RGB(1.0f, 1.0f, 1.0f), 0.0f, 1.0f)
		);

		quad(
			Vertex(projCube[1].x, projCube[1].y, RGB(1.0f, 1.0f, 1.0f), 0.0f, 0.0f),
			Vertex(projCube[5].x, projCube[5].y, RGB(1.0f, 1.0f, 1.0f), 1.0f, 0.0f),
			Vertex(projCube[6].x, projCube[6].y, RGB(1.0f, 1.0f, 1.0f), 1.0f, 1.0f),
			Vertex(projCube[2].x, projCube[2].y, RGB(1.0f, 1.0f, 1.0f), 0.0f, 1.0f)
		);

		quad(
			Vertex(projCube[4].x, projCube[4].y, RGB(1.0f, 1.0f, 1.0f), 0.0f, 0.0f),
			Vertex(projCube[5].x, projCube[5].y, RGB(1.0f, 1.0f, 1.0f), 1.0f, 0.0f),
			Vertex(projCube[1].x, projCube[1].y, RGB(1.0f, 1.0f, 1.0f), 1.0f, 1.0f),
			Vertex(projCube[0].x, projCube[0].y, RGB(1.0f, 1.0f, 1.0f), 0.0f, 1.0f)
		);
	}

	void present() {
		SDL_UnlockTexture(m_buffer);

		SDL_Rect dst = { 0, 0, m_windowWidth, m_windowHeight };
		SDL_RenderCopy(m_renderer, m_buffer, nullptr, &dst);
		SDL_RenderPresent(m_renderer);

		int p;
		SDL_LockTexture(m_buffer, nullptr, (void**) &m_pixels, &p);
	}

	void bindTexture(Texture* tex) {
		m_texture = tex;
	}

	Camera& camera() { return m_camera; }

private:
	int m_windowWidth, m_windowHeight,
		m_bufferWidth, m_bufferHeight;
	SDL_Texture* m_buffer;
	uint8_t* m_pixels;

	SDL_Renderer *m_renderer;
	SDL_Window *m_window;

	Texture* m_texture;
	Camera m_camera{};
};

int main(int argc, char** argv) {
	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window* window = SDL_CreateWindow(
		"SDL Triangle",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		800, 600,
		SDL_WINDOW_SHOWN
	);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	std::unique_ptr<Renderer> ren = std::make_unique<Renderer>(window, renderer);
	std::unique_ptr<Texture> tex = std::make_unique<Texture>("bricks.png");

	ren->bindTexture(tex.get());

	const double timeStep = 1.0 / 120.0;
	double last = double(SDL_GetTicks()) / 1000.0;
	double accum = 0.0;

	int frame = 0;
	double frameTime = 0.0;

	float angle = 0.0f;
	SDL_Event evt;
	bool running = true;
	while (running) {
		bool canRender = false;
		double current = double(SDL_GetTicks()) / 1000.0;
		double delta = current - last;
		last = current;
		accum += delta;

		while (SDL_PollEvent(&evt)) {
			if (evt.type == SDL_QUIT) running = false;
		}

		while (accum >= timeStep) {
			frameTime += timeStep;
			accum -= timeStep;

			if (frameTime >= 1.0) {
				frameTime = 0.0;
				std::string txt = std::to_string(frame) + " fps";
				SDL_SetWindowTitle(window, txt.c_str());
				frame = 0;
			}
			canRender = true;

			angle += timeStep;
		}

		if (canRender) {
			ren->clear(RGB(0.0f, 0.0f, 0.0f));

			ren->camera().angle = angle;
			ren->cube(Vector2(0.0f));

			ren->present();
			frame++;
		}
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}