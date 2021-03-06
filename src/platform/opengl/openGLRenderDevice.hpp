#pragma once

#include "core/window.hpp"
#include "math/color.hpp"

#include "containers/array.hpp"
#include "containers/hash-map.hpp"

#include <SDL2/SDL.h>
#include <GL/glew.h>

#define MAKEFOURCC(a, b, c, d)                              \
                ((uint32)(uint8)(a) | ((uint32)(uint8)(b) << 8) |       \
				((uint32)(uint8)(c) << 16) | ((uint32)(uint8)(d) << 24 ))

#define MAKEFOURCCDXT(a) MAKEFOURCC('D', 'X', 'T', a)

#define FOURCC_DXT1 MAKEFOURCCDXT('1')
#define FOURCC_DXT2 MAKEFOURCCDXT('2')
#define FOURCC_DXT3 MAKEFOURCCDXT('3')
#define FOURCC_DXT4 MAKEFOURCCDXT('4')
#define FOURCC_DXT5 MAKEFOURCCDXT('5')

typedef SDL_GLContext DeviceContext;

class OpenGLRenderDevice {
	public:
		enum BufferUsage {
			USAGE_STATIC_DRAW = GL_STATIC_DRAW,
			USAGE_STREAM_DRAW = GL_STREAM_DRAW,
			USAGE_DYNAMIC_DRAW = GL_DYNAMIC_DRAW,

			USAGE_STATIC_COPY = GL_STATIC_COPY,
			USAGE_STREAM_COPY = GL_STREAM_COPY,
			USAGE_DYNAMIC_COPY = GL_DYNAMIC_COPY,

			USAGE_STATIC_READ = GL_STATIC_READ,
			USAGE_STREAM_READ = GL_STREAM_READ,
			USAGE_DYNAMIC_READ = GL_DYNAMIC_READ,
		};

		enum SamplerFilter {
			FILTER_NEAREST = GL_NEAREST,
			FILTER_LINEAR = GL_LINEAR,
			FILTER_NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
			FILTER_LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
			FILTER_NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
			FILTER_LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR,
		};

		enum SamplerWrapMode {
			WRAP_CLAMP = GL_CLAMP_TO_EDGE,
			WRAP_REPEAT = GL_REPEAT,
			WRAP_CLAMP_MIRROR = GL_MIRROR_CLAMP_TO_EDGE,
			WRAP_REPEAT_MIRROR = GL_MIRRORED_REPEAT,
		};

		enum PixelFormat {
			FORMAT_R,
			FORMAT_RG,
			FORMAT_RGB,
			FORMAT_RGBA,
			FORMAT_DEPTH,
			FORMAT_DEPTH_AND_STENCIL,
		};

		enum PrimitiveType {
			PRIMITIVE_TRIANGLES = GL_TRIANGLES,
			PRIMITIVE_POINTS = GL_POINTS,
			PRIMITIVE_LINE_STRIP = GL_LINE_STRIP,
			PRIMITIVE_LINE_LOOP = GL_LINE_LOOP,
			PRIMITIVE_LINES = GL_LINES,
			PRIMITIVE_LINE_STRIP_ADJACENCY = GL_LINE_STRIP_ADJACENCY,
			PRIMITIVE_LINES_ADJACENCY = GL_LINES_ADJACENCY,
			PRIMITIVE_TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
			PRIMITIVE_TRIANGLE_FAN = GL_TRIANGLE_FAN,
			PRIMITIVE_TRAINGLE_STRIP_ADJACENCY = GL_TRIANGLE_STRIP_ADJACENCY,
			PRIMITIVE_TRIANGLES_ADJACENCY = GL_TRIANGLES_ADJACENCY,
			PRIMITIVE_PATCHES = GL_PATCHES,
		};

		enum FaceCulling {
			FACE_CULL_NONE,
			FACE_CULL_BACK = GL_BACK,
			FACE_CULL_FRONT = GL_FRONT,
			FACE_CULL_FRONT_AND_BACK = GL_FRONT_AND_BACK,
		};

		enum DrawFunc {
			DRAW_FUNC_NEVER = GL_NEVER,
			DRAW_FUNC_ALWAYS = GL_ALWAYS,
			DRAW_FUNC_LESS = GL_LESS,
			DRAW_FUNC_GREATER = GL_GREATER,
			DRAW_FUNC_LEQUAL = GL_LEQUAL,
			DRAW_FUNC_GEQUAL = GL_GEQUAL,
			DRAW_FUNC_EQUAL = GL_EQUAL,
			DRAW_FUNC_NOT_EQUAL = GL_NOTEQUAL,
		};

		enum FramebufferAttachment {
			ATTACHMENT_COLOR = GL_COLOR_ATTACHMENT0,
			ATTACHMENT_DEPTH = GL_DEPTH_ATTACHMENT,
			ATTACHMENT_STENCIL = GL_STENCIL_ATTACHMENT,
		};

		enum BlendFunc {
			BLEND_FUNC_NONE,
			BLEND_FUNC_ONE = GL_ONE,
			BLEND_FUNC_SRC_ALPHA = GL_SRC_ALPHA,
			BLEND_FUNC_ONE_MINUS_SRC_ALPHA = GL_ONE_MINUS_SRC_ALPHA,
			BLEND_FUNC_ONE_MINUS_DST_ALPHA = GL_ONE_MINUS_DST_ALPHA,
			BLEND_FUNC_DST_ALPHA = GL_DST_ALPHA,
		};

		enum StencilOp {
			STENCIL_KEEP = GL_KEEP,
			STENCIL_ZERO = GL_ZERO,
			STENCIL_REPLACE = GL_REPLACE,
			STENICL_INCR = GL_INCR,
			STENCIL_INCR_WRAP = GL_INCR_WRAP,
			STENCIL_DECR_WRAP = GL_DECR_WRAP,
			STENCIL_DECR = GL_DECR,
			STENCIL_INVERT = GL_INVERT,
		};

		enum CubeMapFace {
			CUBE_MAP_POSITIVE_X = GL_TEXTURE_CUBE_MAP_POSITIVE_X,
			CUBE_MAP_NEGATIVE_X = GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
			CUBE_MAP_POSITIVE_Y = GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
			CUBE_MAP_NEGATIVE_Y	= GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
			CUBE_MAP_POSITIVE_Z = GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
			CUBE_MAP_NEGATIVE_Z = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
		};

		enum TextureType {
			TEXTURE_TYPE_2D = GL_TEXTURE_2D,
			TEXTURE_TYPE_CUBE_MAP = GL_TEXTURE_CUBE_MAP
		};

		enum GBufferTextureType {
			GBUFFER_TEXTURE_TYPE_POSITION,
			GBUFFER_TEXTURE_TYPE_DIFFUSE,
			GBUFFER_TEXTURE_TYPE_NORMAL,
			GBUFFER_TEXTURE_TYPE_TEXCOORD,
			GBUFFER_NUM_TEXTURES
		};

		enum FeedbackBufferMode {
			BUFFER_INTERLEAVED_ATTRIBS = GL_INTERLEAVED_ATTRIBS,
			BUFFER_SEPARATE_ATTRIBS = GL_SEPARATE_ATTRIBS
		};

		struct DrawParams {
			enum PrimitiveType primitiveType = PRIMITIVE_TRIANGLES;
			enum FaceCulling faceCulling = FACE_CULL_NONE;
			enum DrawFunc depthFunc = DRAW_FUNC_ALWAYS;
			bool shouldWriteDepth = true;
			bool useStencilTest = false;
			enum DrawFunc stencilFunc = DRAW_FUNC_ALWAYS;
			uint32 stencilTestMask = 0;
			uint32 stencilWriteMask = 0;
			int32 stencilComparisonVal = 0;
			enum StencilOp stencilFail = STENCIL_KEEP;
			enum StencilOp stencilPassButDepthFail = STENCIL_KEEP;
			enum StencilOp stencilPass = STENCIL_KEEP;
			bool useScissorTest = false;
			uint32 scissorStartX = 0;
			uint32 scissorStartY = 0;
			uint32 scissorWidth = 0;
			uint32 scissorHeight = 0;
			enum BlendFunc sourceBlend = BLEND_FUNC_NONE;
			enum BlendFunc destBlend = BLEND_FUNC_NONE;
		};

		struct FeedbackShaderParams {
			char** varyings;
			uint32 numVaryings;
			enum FeedbackBufferMode bufferMode = BUFFER_INTERLEAVED_ATTRIBS;
		};
		
		static bool globalInit();
		OpenGLRenderDevice(Window& window);
		virtual ~OpenGLRenderDevice();

		uint32 createRenderTarget(uint32 texture, int32 width, int32 height,
				enum FramebufferAttachment attachment, uint32
				attachmentNumber, uint32 mipLevel);
		uint32 releaseRenderTarget(uint32 fbo);

		uint32 createGeometryBuffer(int32 width, int32 height);
		uint32 releaseGeometryBuffer(uint32 fbo);

		uint32 createVertexArray(const float** vertexData, const uint32* vertexElementSizes,
				uint32 numVertexComponents, uint32 numInstanceComponents,
				uint32 numVertices, const uint32* indices,
				uint32 numIndices, enum BufferUsage usage);
		void updateVertexArrayBuffer(uint32 vao, uint32 bufferIndex,
				const void* data, uintptr dataSize);
		uint32 releaseVertexArray(uint32 vao);

		uint32 createSampler(enum SamplerFilter minFilter, enum SamplerFilter magFilter,
				enum SamplerWrapMode wrapU, enum SamplerWrapMode wrapV, float anisotropy);
		uint32 releaseSampler(uint32 sampler);

		uint32 createTexture2D(int32 width, int32 height, const void* data,
				enum PixelFormat dataFormat, enum PixelFormat internalFormat,
				bool generateMipmaps, bool compress);
		uint32 createDDSTexture2D(uint32 width, uint32 height, const unsigned char* buffer,
				uint32 fourCC, uint32 mipMapCount);
		uint32 releaseTexture2D(uint32 texture2D);

		uint32 createCubeMap();
		void addTextureToCubeMap(uint32 cubeMap, uint32 face, int32 width, int32 height,
				const void* data);

		void setTexturePackAlignment(uint32 packAlignment);

		uint32 createUniformBuffer(const void* data, uintptr dataSize, enum BufferUsage usage);
		void updateUniformBuffer(uint32 buffer, const void* data, uintptr dataSize);
		void updateUniformBuffer(uint32 buffer, const void* data, uintptr offset, uintptr dataSize);
		uint32 releaseUniformBuffer(uint32 buffer);

		uint32 createShaderProgram(const String& shaderText,
				const FeedbackShaderParams* feedbackParams = nullptr);
		void setShaderUniformBuffer(uint32 shader, const String& uniformBufferName,
				uint32 buffer, uint32 index);
		void setShaderUniformBlockBinding(uint32 shader, const String& uniformBufferName, uint32 block);
		void setShaderSampler(uint32 shader, const String& samplerName,
			uint32 texture, uint32 sampler, uint32 unit, enum TextureType textureType);
		uint32 releaseShaderProgram(uint32 shader);

		uint32 createFeedbackBuffer(const float* initialData, uintptr dataSize,
				uintptr bufferSize, uintptr* attribSizes, uint32 numAttribs);
		void updateFeedbackBuffer(uint32 shader, uint32 feedbackBuffer);
		void releaseFeedbackBuffer(uint32 feedbackBuffer);

		void writeFeedbackBufferData(uint32 feedbackBuffer, const void* data, uintptr dataSize);
		void writeFeedbackBufferData(uint32 feedbackBuffer, const void* data,
				uintptr offset, uintptr dataSize);

		void setClipEnabled(bool enabled, uint32 plane = 0);
		void setRasterizerDiscardEnabled(bool enabled);
		void setDepthMaskEnabled(bool enabled);

		void clear(uint32 fbo,
				bool shouldClearColor, bool shouldClearDepth, bool shouldClearStencil,
				const Color& color, uint32 stencil);
		void draw(uint32 fbo, uint32 shader, uint32 vao, const DrawParams& drawParams,
				uint32 numInstances, uint32 numElements);
		void drawFeedbackBuffer(uint32 fbo, uint32 shader, uint32 feedbackBuffer,
				const DrawParams& drawParams);
	private:
		struct VertexArray {
			uint32* buffers;
			uintptr* bufferSizes;
			uint32  numBuffers;
			uint32  numElements;
			uint32  instanceComponentsStartIndex;
			enum BufferUsage usage;
		};

		struct ShaderProgram {
			Array<uint32> shaders;
			HashMap<String, int32> uniformMap;
			HashMap<String, int32> samplerMap;
		};

		struct FBOData {
			int32 width;
			int32 height;
		};

		struct GBuffer {
			uint32 fbo;
			uint32 textures[GBUFFER_NUM_TEXTURES];
			uint32 depthTexture;
		};

		struct FeedbackBuffer {
			uint32 dataBuffers[2];
			uint32 transformFeedbacks[2];
			
			uintptr* attribSizes;
			uint32 numAttribs;

			uintptr dataSize;
			
			uint32 currVB;
			uint32 currTFB;

			bool firstCall = true;
		};

		static bool isInitialized;

		DeviceContext context;
		String shaderVersion;
		uint32 version;

		HashMap<uint32, VertexArray> vaoMap;
		HashMap<uint32, FBOData> fboMap;
		HashMap<uint32, ShaderProgram> shaderProgramMap;
		HashMap<uint32, GBuffer> geometryBufferMap;
		HashMap<uint32, FeedbackBuffer> feedbackBufferMap;

		uint32 boundFBO;
		uint32 viewportFBO;
		uint32 boundVAO;
		uint32 boundShader;

		enum FaceCulling currentFaceCulling;
		enum DrawFunc currentDepthFunc;
		enum BlendFunc currentSourceBlend;
		enum BlendFunc currentDestBlend;
		enum DrawFunc currentStencilFunc;
		
		uint32 currentStencilTestMask;
		uint32 currentStencilWriteMask;
		int32 currentStencilComparisonVal;
		
		enum StencilOp currentStencilFail;
		enum StencilOp currentStencilPassButDepthFail;
		enum StencilOp currentStencilPass;

		bool blendingEnabled;
		bool shouldWriteDepth;
		bool stencilTestEnabled;
		bool scissorTestEnabled;
		
		void setFBO(uint32 fbo);
		void setViewport(uint32 fbo);
		void setVAO(uint32 vao);
		void setShader(uint32 shader);

		void setFaceCulling(enum FaceCulling faceCulling);
		void setDepthTest(bool shouldWrite, enum DrawFunc depthFunc);
		void setBlending(enum BlendFunc sourceBlend, enum BlendFunc destBlend);
		void setStencilTest(bool enable, enum DrawFunc stencilFunc, uint32 stencilTestMask,
				uint32 stencilWriteMask, int32 stencilComparisonVal, enum StencilOp stencilFail,
				enum StencilOp stencilPassButDepthFail, enum StencilOp stencilPass);
		void setStencilWriteMask(uint32 mask);
		void setScissorTest(bool enable, uint32 startX = 0, uint32 startY = 0,
				uint32 width = 0, uint32 height = 0);

		uint32 getVersion();
		String getShaderVersion();

		NULL_COPY_AND_ASSIGN(OpenGLRenderDevice)
};
