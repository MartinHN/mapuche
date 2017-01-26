

//
//  ofxProjectorBlendShader.h
//  twoScreens
//
//  Created by Jeffrey Crouse on 11/23/12.
//
//  ofxProjectorBlendVertShader and ofxProjectorBlendFragShader


#pragma once
#define STRINGIFY(A) #A

string ofxProjectorBlendVertShader = STRINGIFY(
                                               void main(void) {
                                                 gl_Position = ftransform();
                                                 gl_TexCoord[0] = gl_MultiTexCoord0;
                                               });




string ofxProjectorBlendFragShader(int blends) {
  string shader =
  STRINGIFY(
            uniform float BlendPower;
            uniform float SomeLuminanceControl;
            uniform float GammaCorrection;
            uniform sampler2DRect Tex0;
            uniform vec2  texCoordOffset;
            uniform float width;
            uniform float height;
            uniform float OverlapLeft;
            uniform float OverlapRight;
            uniform float OverlapTop;
            uniform float OverlapBottom;

//            uniform float BlackOutLeft;
//            uniform float BlackOutRight;
//            uniform float BlackOutTop;
//            uniform float BlackOutBottom;

            uniform float SolidEdgeEnable;
            uniform vec4  SolidEdgeColor;






            vec4 drawSmoothEdge(float loc, float extend, float blePow, float lumCon, vec3 gamma)
            {
              vec4 edge;
              float curve;

              curve = loc / extend;
              curve = (curve < 0.5) ?
              lumCon * pow(2.0 * curve, blePow)
              :
              1.0 - (1.0 - lumCon) * pow(2.0 * (1.0 - curve), blePow);

              edge = vec4(pow(curve, 1.0 / gamma.r),
                          pow(curve, 1.0 / gamma.g),
                          pow(curve, 1.0 / gamma.b),
                          1.0);

              return edge;
            }

            vec4 drawSmoothEdges(in vec4 overlap, in vec4 blankout, in vec4 color)
            {
              vec2 xy = gl_TexCoord[0].xy;
              vec2 xy_t = xy+ texCoordOffset.xy;
              vec4 blank = vec4(0.0,0.0,0.0,0.0);

              /*
               float proj2 = 1.0; //projectors ... this basically defines a limit;
               float thresh = proj2 * width + overlap.x; //(projectors - overlap.x) * width;
               float blnPwr = (xy_t.x > thresh) ?  BlendPower[1] : BlendPower[0];
               float slc    = (xy_t.x > thresh) ?  SomeLuminanceControl[1] : SomeLuminanceControl[0];
               float _g   = (xy_t.x > thresh) ?  GammaCorrection[1] : GammaCorrection[0];
               vec3 gamma = vec3(_g, _g, _g);
               */


              float blnPwr = BlendPower;
              float slc = SomeLuminanceControl + 0.5;
              vec3 gamma = vec3(GammaCorrection, GammaCorrection, GammaCorrection);
              //slc += 0.5;

              vec4 sebImage =
              (blankout.x + overlap.x > xy.x) ?
              (blankout.x > xy.x) ?
              blank 							//leftBlankout
              :	(blankout.z + overlap.z > xy.y) ?
              (blankout.z > xy.y) ?
              blank					//leftBottomBlankOut
              : 	texture2DRect(Tex0, xy_t) 	*		//leftBottomBlend
              drawSmoothEdge(xy.x - blankout.x, overlap.x, blnPwr, slc, gamma) *
              drawSmoothEdge(xy.y - blankout.z, overlap.z, blnPwr, slc, gamma)
              : 	(height - blankout.w - overlap.w < xy.y) ?
              (height - blankout.w < xy.y) ?
              blank				//leftTopBlankout
              : 	texture2DRect(Tex0, xy_t) 	*	//LeftTopBlend
              drawSmoothEdge(xy.x - blankout.x, overlap.x, blnPwr, slc, gamma) *
              drawSmoothEdge(height - blankout.w - xy.y, overlap.w, blnPwr, slc, gamma)
              :	texture2DRect(Tex0, xy_t) *		//leftBlankout
              drawSmoothEdge(xy.x - blankout.x, overlap.x, blnPwr, slc, gamma)// + vec4(0.5, 0, 0.5, 0)	// debug: magenta
              :	(width - blankout.y - overlap.y < xy.x) ?
              (width - blankout.y < xy.x) ?
              blank						//rightBlankout
              :	(blankout.z + overlap.z > xy.y) ?
              (blankout.z > xy.y) ?
              blank 				//rightBottomBlankout
              : 	texture2DRect(Tex0, xy_t) 	*	//rightBottomBlend
              drawSmoothEdge(width - blankout.y - xy.x, overlap.y, blnPwr, slc, gamma) *
              drawSmoothEdge(xy.y - blankout.z, overlap.z, blnPwr, slc, gamma)
              :	(height - blankout.w - overlap.w < xy.y) ?
              (height - blankout.w < xy.y) ?
              blank 			//rightTopBlankout
              :	texture2DRect(Tex0, xy_t) * //rightTopBlend
              drawSmoothEdge(width - blankout.y - xy.x, overlap.y, blnPwr, slc, gamma) *
              drawSmoothEdge(height - blankout.w - xy.y, overlap.w, blnPwr, slc, gamma)
              :	texture2DRect(Tex0, xy_t) * 	//rightBlend
              drawSmoothEdge(width - blankout.y - xy.x, overlap.y, blnPwr, slc, gamma)// + vec4(0, 0, 0.5, 0)	// debug: blue
              :	(blankout.z + overlap.z > xy.y) ?
              (blankout.z > xy.y) ?
              blank 					//BottomBlankout
              : 	texture2DRect(Tex0, xy_t) * 		//BottomBlend
              drawSmoothEdge(xy.y - blankout.z, overlap.z, blnPwr, slc, gamma)
              :	(height - blankout.w - overlap.w < xy.y) ?
              (height - blankout.w < xy.y) ?
              blank 				//TopBlankout
              :	texture2DRect(Tex0, xy_t) * 	//TopBlend
              drawSmoothEdge(height - blankout.w - xy.y, overlap.w, blnPwr, slc, gamma)
              :	texture2DRect(Tex0, xy_t);// + vec4(0, 0.5, 0.5, 0);	// debug: teal

              //	if(xy_t.x < thresh) sebImage.rgb += vec3(0, 1.0, 0);	// debug

              return sebImage;
            }

            vec4 drawSolidEdges(vec4 overlap, vec4 blankout, vec4 color)
            {
              vec2 xy = gl_TexCoord[0].xy;
              vec2 xy_t = xy+ texCoordOffset.xy;;

              vec4 blank = vec4(0.0,0.0,0.0,0.0);

              vec4 edgeImage =
              (blankout.x + overlap.x > xy.x) ?
              (blankout.x > xy.x) ?
              blank				//leftBlankout
              :	(blankout.z + overlap.z > xy.y) ?
              (blankout.z > xy.y) ?
              blank			//leftBottomBlankout
              : 	color			//leftBottomColor
              : 	(height - blankout.w - overlap.w < xy.y) ?
              (height - blankout.w < xy.y) ?
              blank 		//leftTop
              : 	color 		//leftTopColor
              :	color			//leftColor
              :	(width - blankout.y - overlap.y < xy.x) ?
              (width - blankout.y < xy.x) ?
              blank				//rightBlankout
              :	(blankout.z + overlap.z > xy.y) ?
              (blankout.z > xy.y) ?
              blank 		//rightBottomBlankout
              : 	color		//rightBottomColor
              :	(height - blankout.w - overlap.w < xy.y) ?
              (height - blankout.w < xy.y) ?
              blank 	//rightTopBlankout
              :	color 	//rightToColor
              :	color 		//rightColor
              :	(blankout.z + overlap.z > xy.y) ?
              (blankout.z > xy.y) ?
              blank 			//BottomBlankout
              : 	color			//BottomColor
              :	(height - blankout.w - overlap.w < xy.y) ?
              (height - blankout.w < xy.y) ?
              blank 		//TopBlankout
              :	color		//TopColor
              :	texture2DRect(Tex0, xy_t);
              return edgeImage;
            }

            void main (void)
            {
              vec4 overlap = vec4 (OverlapLeft, OverlapRight, OverlapBottom, OverlapTop);
              vec4 blankout = vec4 (0,0,0,0);//BlackOutLeft, BlackOutRight, BlackOutBottom, BlackOutTop);
              if (SolidEdgeEnable == 1.0) {
                gl_FragData[0] =drawSolidEdges(overlap, blankout, SolidEdgeColor);
              }
              else{
                gl_FragData[0]=drawSmoothEdges(overlap, blankout, SolidEdgeColor);
              }
            }
            );
  return shader;
};

string ofxProjectorBlendFragShader2(int blends) {
  string shader =
  STRINGIFY(
            uniform sampler2DRect Tex0;
            uniform vec2  texCoordOffset;
            // used in a few inversions
            const vec3 one = vec3(1.0);
            /*
            //const vec3 gamma = vec3(1.8, 1.5, 1.2);
            //const vec3 luminance = vec3(0.5);
*/
            // controls the interpolation curve ([1..n], 1.0 = linear, 2.0 = default quadratic)
            uniform float BlendPower; // try: 2.0;
            // controls the center of interpolation ([0..1], 0.5 = linear)
            uniform float SomeLuminanceControl; // try: vec3(0.5);
            /*
            // controls gamma levels ([1..n], 1.8 or 2.2 is typical)
            //uniform vec3 gamma; // try: vec3(1.8, 1.5, 1.2);
            // controls blending area at left, top, right and bottom in percentages ([0..0.5])
//            uniform vec4 amount;
             */
            uniform float OverlapLeft;
            uniform float OverlapRight;
            uniform float OverlapTop;
            uniform float OverlapBottom;
            //uniform vec4 edges; // try: vec4(0.4, 0.4, 0.0, 0.0);

            uniform float width;
            uniform float height;
            uniform float GammaCorrection;

            void main(){
              /*
              //this is the fragment shader
              //this is where the pixel level drawing happens
              //gl_FragCoord gives us the x and y of the current pixel its drawing

              //we grab the x and y and store them in an int
*/
              vec2 uv = gl_TexCoord[0].xy-texCoordOffset;
              //vec4 col = texture2D(tex,uv).xyz;
              vec4 col = texture2DRect(Tex0,gl_TexCoord[0].xy / vec2(1));//+texCoordOffset);
              /*
              //vec2 uv = vec2(gl_FragCoord.x, gl_FragCoord.y);
              //vec4 col = gl_Color;
               */
              vec4 edges = vec4(OverlapLeft, OverlapTop, OverlapRight, OverlapBottom);
              //vec3 gamma = vec3(GammaCorrection, 1.5, 1.2);
              vec3 gamma = vec3(GammaCorrection, GammaCorrection, GammaCorrection);
              vec3 luminance = vec3(SomeLuminanceControl + 0.5);




              // calculate edge blending factor
              float a = 1.0;

              if(edges.x > 0.0) a *= clamp(uv.x*1.0/edges.x, 0.0, 1.0);
              if(edges.y > 0.0) a *= clamp(uv.y*1.0/edges.y, 0.0, 1.0);
              if(edges.z > 0.0) a *= clamp(((width)-uv.x)*1.0/edges.z, 0.0, 1.0);
              if(edges.w > 0.0) a *= clamp(((height)-uv.y)*1.0/edges.w, 0.0, 1.0);

              // blend function with luminance control (for each of the 3 channels)
              vec3 blend = (a < 0.5) ? (luminance * pow(2.0 * a, BlendPower))
              : one - (one - luminance) * pow(2.0 * (1.0 - a), BlendPower);
              
              // gamma correction (for each of the 3 channels)
//              blend = pow(blend, one / gamma);

              // set final color
              //gl_FragColor = vec4(col * blend, 1.0);


              gl_FragColor = vec4(col.xyz*a,1);// col.w*blend.x);

//              gl_FragData[0] = vec4(col.xyz*blend, col.w*blend.x);

//              gl_FragColor = vec4(col.xyz*uv.x/width , 1.0);
            }

  );
  return shader;
};

