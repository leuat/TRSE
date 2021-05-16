export PLANTUML_LIMIT_SIZE=32000 
plantuml output.puml -theme crt-amber
mv output.png generated_compiler.png
open generated_compiler.png
