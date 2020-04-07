# Spacerace 69 - Evoke 2k20 invitation by Team210 shown at Revision 2k20
# Copyright (C) 2019  Alexander Kraus <nr4@z10.info>
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

import argparse, json, os.path
import GLSLLexer130, Compressor

def authorJSON(authorName, demoJSON):
    for author in demoJSON['authors']:
        if author['handle'] == authorName:
            return author
    print("Requested author does not exist.")
    return 0

def tokenIs(token, identifier):
    if token != None:
        if token.tokenName == identifier:
            return True
    return False

def tokenIsDataType(token):
    dataTypeTokenNames = [ "VOID", "FLOAT", "VEC2", "VEC3", "VEC4", "MAT2", "MAT3", "MAT4", "BVEC2", "BVEC3", "BVEC4", "UVEC2", "UVEC3", "UVEC4", "INT" ]
    if token != None:
        if token.tokenName in dataTypeTokenNames:
            return True
    return False

def hasEntryPoint(source):
    lexer = GLSLLexer130.GLSLLexer130(source)
    token = lexer.token()
    while token != None:
        if tokenIs(token, "VOID"):
            token = lexer.token()
            if tokenIs(token, "MAIN"):
                token = lexer.token()
                if tokenIs(token, "LPAREN"):
                    token = lexer.token()
                    if tokenIs(token, "RPAREN"):
                        return True
        token = lexer.token()
    return False

def containedSymbolPrototypes(source):
    lexer = GLSLLexer130.GLSLLexer130(source)
    token = lexer.token()
    symbolList = []
    
    while token != None:
        if tokenIsDataType(token):
            token = lexer.token()
            if tokenIs(token, "IDENTIFIER"):
                symbolIdentifier = token.tokenData
                token = lexer.token()
                if tokenIs(token, "LPAREN"):
                    token = lexer.token()
                    if tokenIs(token, "RPAREN"):
                        token = lexer.token()
                        if tokenIs(token, "SEMICOLON"):
                            symbolList += [ symbolIdentifier ]
                    else:
                        # Ignore the now following argument list; it does not matter at all.
                        while not tokenIs(token, "RPAREN"):
                            token = lexer.token()
                        token = lexer.token()
                        if tokenIs(token, "SEMICOLON"):
                            symbolList += [ symbolIdentifier ]
        token = lexer.token()
    return symbolList

def tokenNeedsSpace(token):
    tokenNamesWithSpace = [ "VOID", "FLOAT", "VEC2", "VEC3", "VEC4", "MAT2", "MAT3", "MAT4", "SAMPLER2D", "UNIFORM", "IN_QUALIFIER", "OUT_QUALIFIER", "INOUT_QUALIFIER", "VOID", "VERSION_DIRECTIVE", "DEFINE_DIRECTIVE", "CONST", "INT", "ELSE", "RETURN" ]
    if token != None:
        if token.tokenName in tokenNamesWithSpace:
            return True
    return False

def compressSource(source):
    lexer = GLSLLexer130.GLSLLexer130(source)
    token = lexer.token()
    smallerSource = ""
    
    while token != None:
        if (not tokenIs(token, "SINGLELINE_COMMENT")) and (not tokenIs(token, "MULTILINE_COMMENT")):
            smallerSource += token.tokenData
            if tokenNeedsSpace(token):
                smallerSource += ' '     
        print(token.tokenName, token.tokenData)
        token = lexer.token()
    
    return smallerSource

parser = argparse.ArgumentParser(description='Team210 generator tool.')
parser.add_argument('--debug', dest='debug', action='store_true')
args, rest = parser.parse_known_args()

if rest == []:
    print('No input file present. Can not do anything.')
    exit()

f = open(rest[0], 'rt')
demoJSON = json.load(f)
f.close()

demoId = demoJSON['identifier']
demoGroup = demoJSON['group']
demoParty = demoJSON['party']
partyYear = demoJSON['year']
demoName = demoJSON['name']
demoLength = demoJSON['length']

# Scan for shader files needed for loading bar
loadingBarFilename = demoJSON['loading bar']
f = open('symbols/' + loadingBarFilename, "rt")
loadingBarShaderSource = f.read()
f.close()
loadingBarSymbols = containedSymbolPrototypes(loadingBarShaderSource)
f = open("LoadingBar.gen.cpp", "wt")
f.write("#include \"LoadingBar.hpp\"\n")
f.write("\n")
f.write("void LoadingBar::compileSymbolsForSelf()\n")
f.write("{\n")
for symbol in loadingBarSymbols:
    g = open("symbols\\" + symbol + ".frag", "rt")
    symbolSource = g.read()
    g.close()
    compressedSymbolSource = compressSource(symbolSource)
    print(compressedSymbolSource)
    #f.write("\tsymbolTable->addSymbol(new Shader(" + compressedSymbolSource + "
#f.write("\tsymbolTable->addSymbol(
f.close()

# Scan for all shader files
#shaderFiles = []
#for sceneJSON in demoJSON['scenes']:
    #for scenePass in sceneJSON['passes']:
        #shaderFiles += [ 'gfx/' + scenePass['fragment'] ]
#shaderFiles = list(set(shaderFiles))

# Load default shader file template
#f = open('generator/shader.frag.template', 'rt')
#shaderTemplate = f.read().replace('$DEMONAME', demoName).replace('$DEMOGROUP', demoGroup).replace('$DEMOPARTY', demoParty).replace('$PARTYYEAR', str(partyYear))
#f.close()

# Check if shader files exist, if not, generate from template
#for shaderFile in shaderFiles:
    #if not os.path.exists(shaderFile):
        #f = open(shaderFile, 'wt')
        #f.write(shaderTemplate)
        #f.close()

#### Generate :/config.gen.cmake
f = open('config.gen.cmake', 'wt')
f.write('set(DEMO_IDENTIFIER ' + demoId + ')' + '\n')
#f.write('set(SHADER_FILES\n')
#for shaderFile in shaderFiles:
    #f.write('    ' + shaderFile + '\n')
#f.write(')\n')
f.close()

# Generate author list
authorList = ""
centeredAuthorList = ""
for authorJSON in demoJSON['authors']:
    authorLine = authorJSON['handle'] + ' - fancy '
    for credit in authorJSON['credits'][:-1]:
        authorLine += credit + ' ^ '
    authorLine += authorJSON['credits'][-1] + '\n'
    authorList += authorLine
    centeredAuthorList += '!' + authorLine 
    
# Generate description
centeredDescriptionList = ""
for description in demoJSON['description']:
    centeredDescriptionList += description.center(50) + '\n'

# Load NFO template
f = open('generator/demogroup.nfo.template', mode='r', encoding='utf-8')
nfoTemplate = f.read().replace('$DEMONAME', demoName).replace('$DEMOGROUP', demoGroup).replace('$DEMOPARTY', demoParty).replace('$PARTYYEAR', str(partyYear)).replace('$AUTHORLIST', centeredAuthorList)
nfoLines = nfoTemplate.split('\n')
nfoTemplate = ""
for line in nfoLines:
    if line.startswith('!'):
        nfoTemplate += line[1:].center(50) + '\n'
    else:
        nfoTemplate += line + '\n'
nfoTemplate = nfoTemplate.replace('$DESCRIPTION', centeredDescriptionList)
f.close()

#### Generate :/[demogroup].nfo
f = open(demoGroup + ".nfo", mode="w")
f.write(nfoTemplate)
f.close()

#### Generate common.gen.h with global settings like demo name, duration, etc.
#f = open('common.gen.h', 'wt')
#f.write('#ifndef COMMON_GEN_H\n')
#f.write('#define COMMON_GEN_H\n')
#f.write('const char *demoname = \"' + demoName + ' / ' + demoGroup + '\";\n')
#f.write('#define duration (' + str(demoLength) + ')\n')
#f.write('const double duration1 = duration;\n')
#f.write('#endif\n')
#f.close()

#### Generate readme.md with up-to-date information
f = open('readme.md', 'wt')
f.write('# ' + demoName + '\n')
f.write('PC-64k-Intro by ' + demoGroup + ' at ' + demoParty + ' ' + str(partyYear) + '\n\n')
f.write('# Licenses\n')
for copyrightNotice in demoJSON['license']:
    f.write('- '+copyrightNotice.replace('YEAR', str(partyYear)).replace('GROUP', demoGroup) + '\n')
f.write('\n# Contributing members of Team210\n')
for author in demoJSON['authors']:
    line = author['handle'] + ' - '
    for credit in author['credits'][:-1]:
        line += credit + ' ^ '
    line += author['credits'][-1]
    f.write('- '+line+'\n')
f.close()

#### Generate scenes.gen.h with scene information.
# Generate framebuffer and texture setup.
# Number of textures is highest number in input
# Number of framebuffers is highest number in output
#numTextures = 0
#numFramebuffers = 0
#for sceneJSON in demoJSON['scenes']:
    #for passJSON in sceneJSON['passes']:
        #if passJSON['input textures'] == []: continue
        #numTextures = max(numTextures, max(passJSON['input textures']))
        #numFramebuffers = max(numFramebuffers, passJSON['output texture'])
#numTextures += 1
#numFramebuffers += 1

# Write actual file
#f = open('scenes.gen.h', 'wt')
#f.write('#ifndef SCENES_GEN_H\n')
#f.write('#define SCENES_GEN_H\n')
#f.write('\n')
#f.write('GLuint pass_textures[' + str(numTextures) + '], pass_framebuffers[' + str(numFramebuffers) + '];\n')
#f.write('\n')
#f.write('void create_render_framebuffers()\n')
#f.write('{\n')
#f.write('    glGenFramebuffers(' + str(numFramebuffers) + ', pass_framebuffers);\n')
#f.write('    glGenTextures(' + str(numTextures) + ', pass_textures);\n')
#for i in range(numTextures):
    #f.write('    glBindTexture(GL_TEXTURE_2D, pass_textures[' + str(i) + ']);\n')
    #f.write('    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);\n')
    #f.write('    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);\n')
#f.write('}\n')
#f.write('\n')
#for i in range(len(demoJSON['scenes'])):
    #sceneJSON = demoJSON['scenes'][i]
    #f.write('void setup_scene_' + str(i) + '_rendergraph()\n')
    #f.write('{\n')
    #for j in range(len(sceneJSON['passes'])):
        #passJSON = sceneJSON['passes'][j]
        #if passJSON['output texture'] == -1: continue
        #f.write('    glBindFramebuffer(GL_FRAMEBUFFER, pass_framebuffers[' + str(passJSON['output texture']) + ']);\n')
        #f.write('    glBindTexture(GL_TEXTURE_2D, pass_textures[' + str(passJSON['output texture']) + ']);\n')
        #f.write('    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);\n')
        #f.write('    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pass_textures[' + str(passJSON['output texture']) + '], 0);\n')
        #f.write('    glDrawBuffer(GL_COLOR_ATTACHMENT0);\n')
    #f.write('}\n')
    #f.write('\n')
    #f.write('#define t_' + str(i) + ' (' + str(sceneJSON['start']) + ')\n')
#f.write('const double start_times[] = {\n')
#for i in range(len(demoJSON['scenes'][:-1])):
    #f.write('    t_' + str(i) + ',\n')
#f.write('    t_' + str(len(demoJSON['scenes'])-1) + '\n};\n')
#f.write('const char *scene_names[] = {\n')
#for sceneJSON in demoJSON['scenes'][:-1]:
    #f.write('    \"' + sceneJSON['name'] + '\",')
#f.write('    \"' + demoJSON['scenes'][-1]['name'] + '\"\n};\n')
#f.write('const unsigned int nscenes = ARRAYSIZE(start_times);\n')
#f.write('#endif\n')
#f.close()

#### Generate draw.gen.h with scene drawing information.
# When a scene gets drawn for the first time, call setup_scene_x_rendergraph()
# select proper framebuffers, input textures, shader and uniforms for each pass.
#f = open('draw.gen.h', 'wt')
#f.write('#ifndef DRAW_GEN_H\n')
#f.write('#define DRAW_GEN_H\n')
#for i in range(len(demoJSON['scenes'])):
    #sceneJSON = demoJSON['scenes'][i]
    
#f.write('#endif\n')
#f.close()

#### Generate draw.gen.h with drawing code
#f = open('draw.gen.h', 'wt')
#f.write('#ifndef DRAW_GEN_H\n')
#f.write('#define DRAW_GEN_H\n')
#for i in range(len(demoJSON['scenes'])):
    #sceneJSON = demoJSON['scenes'][i]
    #sceneIdentifier = sceneJSON['fragment'].replace('.frag', '')
    #scenePasses = sceneJSON['passes']
    
    #if i > 0: 
        #f.write('else ')
    #if i < len(demoJSON['scenes'])-1:
        #f.write('if(t < t_' + demoJSON['scenes'][i+1]['fragment'].replace('.frag', '') + ')\n')
    #f.write('{\n')
    
    ## Switch buffer setup if frame is first frame in scene
    #f.write('    if(t_last_frame < t_' + sceneJSON['fragment'].replace('.frag', '') + ')\n')
    #f.write('    {\n')
    #for scenePass in scenePasses:
        #f.write('        glBindFramebuffer(GL_FRAMEBUFFER, ' + scenePass['
    #f.write('    }\n')
    
    ## Use matching shader program and update uniforms
    #f.write('    glUseProgram(shader_program_gfx_' + sceneIdentifier + '.handle);\n')
    #if 'time offset' in sceneJSON:
            #f.write('    glUniform1f(shader_uniform_gfx_' + sceneIdentifier + '_iTime, t - t_' + sceneIdentifier + ' + ' + str(sceneJSON['time offset']) + ');\n')
    #else:
        #f.write('    glUniform1f(shader_uniform_gfx_' + sceneIdentifier + '_iTime, t - t_' + sceneIdentifier + ');\n')
    #f.write('    glUniform2f(shader_uniform_gfx_' + sceneIdentifier + '_iResolution, w, h);\n')
    #f.write('#ifdef MIDI\n')
    #for j in range(8):
        #f.write('    glUniform1f(shader_uniform_gfx_' + sceneIdentifier + '_iFader' + str(j) + ', fader' + str(j) + ');\n')
    #f.write('#endif\n')
    #f.write('}\n')
#f.write('#endif\n')
#f.close()
