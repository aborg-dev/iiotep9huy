package model;

import static java.lang.StrictMath.*;

/**
 * Базовый класс для определения объектов (<<юнитов>>) на игровом поле.
 */
public abstract class Unit {
    private final long id;
    private final double width;
    private final double height;
    private final double x;
    private final double y;
    private final double speedX;
    private final double speedY;
    private final double angle;
    private final double angularSpeed;

    protected Unit(
            long id, double width, double height, double x, double y,
            double speedX, double speedY, double angle, double angularSpeed) {
        this.id = id;
        this.width = width;
        this.height = height;
        this.x = x;
        this.y = y;
        this.speedX = speedX;
        this.speedY = speedY;
        this.angle = angle;
        this.angularSpeed = angularSpeed;
    }

    /**
     * @return Возвращает уникальный идентификатор объекта.
     */
    public long getId() {
        return id;
    }

    /**
     * @return Возвращает ширину объекта.
     */
    public double getWidth() {
        return width;
    }

    /**
     * @return Возвращает высоту объекта.
     */
    public double getHeight() {
        return height;
    }

    /**
     * @return Возвращает X-координату центра объекта. Ось абсцисс направлена слева направо.
     */
    public final double getX() {
        return x;
    }


    /**
     * @return Возвращает Y-координату центра объекта. Ось ординат направлена свеху вниз.
     */
    public final double getY() {
        return y;
    }

    /**
     * @return Возвращает X-составляющую скорости объекта. Ось абсцисс направлена слева направо.
     */
    public final double getSpeedX() {
        return speedX;
    }

    /**
     * @return Возвращает Y-составляющую скорости объекта. Ось ординат направлена свеху вниз.
     */
    public final double getSpeedY() {
        return speedY;
    }

    /**
     * @return Возвращает угол поворота объекта в радианах. Нулевой угол соответствует направлению оси абсцисс.
     *         Положительные значения соответствуют повороту по часовой стрелке.
     */
    public final double getAngle() {
        return angle;
    }

    /**
     * @return Возвращает скорость вращения объекта.
     *         Положительные значения соответствуют вращению по часовой стрелке.
     */
    public double getAngularSpeed() {
        return angularSpeed;
    }

    /**
     * Returns the relative angle in range of {@code -PI} to {@code PI} both inclusive.
     *
     * @param x the abscissa coordinate of the point
     * @param y the ordinate coordinate of the point
     * @return angle to the point relative to this unit's angle
     */

    /**
     * @param x X-координата точки.
     * @param y Y-координата точки.
     * @return Возвращает ориентированный угол [{@code -PI}, {@code PI}] между направлением
     *         данного объекта и вектором из центра данного объекта к указанной точке.
     */
    public double getAngleTo(double x, double y) {
        double absoluteAngleTo = atan2(y - this.y, x - this.x);
        double relativeAngleTo = absoluteAngleTo - angle;

        while (relativeAngleTo > PI) {
            relativeAngleTo -= 2.0D * PI;
        }

        while (relativeAngleTo < -PI) {
            relativeAngleTo += 2.0D * PI;
        }

        return relativeAngleTo;
    }

    /**
     * Returns the relative angle in range of {@code -PI} to {@code PI} both inclusive.
     *
     * @param unit unit to calculate relative angle to
     * @return angle to the center of specified unit relative to this unit's angle
     */

    /**
     * @param unit Объект, к центру которого необходимо определить угол.
     * @return Возвращает ориентированный угол [{@code -PI}, {@code PI}] между направлением
     *         данного объекта и вектором из центра данного объекта к центру указанного объекта.
     */
    public double getAngleTo(Unit unit) {
        return getAngleTo(unit.x, unit.y);
    }

    /**
     * Returns the distance to the point.
     *
     * @param x the abscissa coordinate of the point
     * @param y the ordinate coordinate of the point
     * @return distance to the point from the center of the unit
     */

    /**
     * @param x X-координата точки.
     * @param y Y-координата точки.
     * @return Возвращает расстояние до точки от центра данного объекта.
     */
    public double getDistanceTo(double x, double y) {
        return hypot(x - this.x, y - this.y);
    }

    /**
     * Returns the distance to the unit's center.
     *
     * @param unit unit to calculate distance to
     * @return distance to the center of specified unit from the center of this unit
     */

    /**
     * @param unit Объект, до центра которого необходимо определить расстояние.
     * @return Возвращает расстояние от центра данного объекта до центра указанного объекта.
     */
    public double getDistanceTo(Unit unit) {
        return getDistanceTo(unit.x, unit.y);
    }
}
