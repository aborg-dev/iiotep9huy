if not exist src\main\java\Runner.java (
    echo Unable to find src\main\java\Runner.java > compilation.log
    exit 1
)

rd /Q /S classes
md classes

javac -sourcepath "src/main/java" -d classes "src/main/java/Runner.java" > compilation.log

if not exist classes\Runner.class (
    echo Unable to find classes\Runner.class > compilation.log
    exit 1
)

jar cf "./java-cgdk.jar" -C "./classes" .
