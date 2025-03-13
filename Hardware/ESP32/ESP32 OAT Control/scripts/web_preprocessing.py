from SCons.Script import Import
import sys
import subprocess

Import("env")

def check_package_installed(package):
    try:
        __import__(package)
        return True
    except ImportError:
        return False

# bs4가 설치되어 있지 않은 경우에만 설치
if not check_package_installed('bs4'):
    env.Execute("$PYTHONEXE -m pip install beautifulsoup4")

from bs4 import BeautifulSoup

def web_preprocessing(source, target, env):
    # HTML 파일 읽기
    with open("html/index.html", "r", encoding="utf-8") as f:
        html_content = f.read()
    
    # CSS 파일 읽기
    with open("html/css/style.css", "r", encoding="utf-8") as f:
        css_content = f.read()
    
    # JS 파일 읽기
    with open("html/js/script.js", "r", encoding="utf-8") as f:
        js_content = f.read()

    # HTML 파싱
    html_parse = BeautifulSoup(html_content, 'html.parser')

    # style
    link_tag = html_parse.find('link', {'rel': 'stylesheet'})
    if link_tag:
        style_tag = html_parse.new_tag('style')
        style_tag.string = '%STYLE_PLACEHOLDER%'
        link_tag.replace_with(style_tag)
    
    # script
    script_tag = html_parse.find('script')
    script_tag.string = '%SCRIPT_PLACEHOLDER%'
    
    # footer
    footer = html_parse.find('footer')
    footer.find('div').find_all('p')[0].string = '%GPSINFO%'
    footer.find('div').find_all('p')[1].string = '%FIRMWAREMODEL%' 
    footer.find('div').find_all('p')[2].string = '%FIRMWAREVERSION%'
    
    # page.hpp 생성
    with open("inc/page.hpp", "w", encoding="utf-8") as f:
        f.write("#pragma once\n\n")
        f.write("#include <ESPAsyncWebServer.h>\n\n")
        
        # CSS 문자열
        f.write("const char style_css[] PROGMEM = R\"rawliteral(\n")
        f.write(css_content.replace("%", "%%"))
        f.write("\n)rawliteral\";\n\n")
        
        # JS 문자열
        f.write("const char script_js[] PROGMEM = R\"rawliteral(\n")
        f.write(js_content)
        f.write("\n)rawliteral\";\n\n")
        
        # HTML 문자열
        f.write("const char index_html[] PROGMEM = R\"rawliteral(\n")
        f.write(str(html_parse))
        f.write("\n)rawliteral\";\n")
        
env.AddPreAction("buildprog", web_preprocessing)
