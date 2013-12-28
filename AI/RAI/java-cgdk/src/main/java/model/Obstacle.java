package model;

/**
 * Класс, определяющий препятствие --- неподвижный неразрушимый объект.
 */
public final class Obstacle extends Unit {
    public Obstacle(long id, double width, double height, double x, double y) {
        super(id, width, height, x, y, 0.0D, 0.0D, 0.0D, 0.0D);
    }
}
