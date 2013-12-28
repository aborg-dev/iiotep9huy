package model;

import static java.lang.StrictMath.PI;
import static java.lang.StrictMath.atan2;

/**
 * Класс, определяющий танк. Содержит также все свойства юнита.
 */
public final class Tank extends Unit {
    private final String playerName;
    private final int teammateIndex;
    private final double turretRelativeAngle;
    private final int crewHealth;
    private final int hullDurability;
    private final int reloadingTime;
    private final int remainingReloadingTime;
    private final int premiumShellCount;
    private final boolean teammate;
    private final TankType type;

    public Tank(
            long id, String playerName, int teammateIndex,
            double x, double y, double speedX, double speedY, double angle, double angularSpeed,
            double turretRelativeAngle, int crewHealth, int hullDurability,
            int reloadingTime, int remainingReloadingTime, int premiumShellCount, boolean teammate, TankType type) {
        super(id, type.getWidth(), type.getHeight(), x, y, speedX, speedY, angle, angularSpeed);

        this.playerName = playerName;
        this.teammateIndex = teammateIndex;
        this.turretRelativeAngle = turretRelativeAngle;
        this.crewHealth = crewHealth;
        this.hullDurability = hullDurability;
        this.reloadingTime = reloadingTime;
        this.remainingReloadingTime = remainingReloadingTime;
        this.premiumShellCount = premiumShellCount;
        this.teammate = teammate;
        this.type = type;
    }

    /**
     * @return Возвращает имя игрока --- владельца танка.
     */
    public String getPlayerName() {
        return playerName;
    }

    /**
     * @return Возвращает 0-индексированный номер танка в команде.
     */
    public int getTeammateIndex() {
        return teammateIndex;
    }

    /**
     * @return Возвращает угол (в радианах) поворота башни относительно танка.
     */
    public double getTurretRelativeAngle() {
        return turretRelativeAngle;
    }

    /**
     * @return Возвращает текущее здоровье экипажа.
     */
    public int getCrewHealth() {
        return crewHealth;
    }

    /**
     * @return Возвращает текущую прочность корпуса.
     */
    public int getHullDurability() {
        return hullDurability;
    }

    /**
     * @return Возвращает количество времени в тиках, необходимое для выполнения полной перезарядки орудия.
     */
    public int getReloadingTime() {
        return reloadingTime;
    }

    /**
     * @return Возвращает количество времени в тиках, оставшееся до следующего возможного выстрела.
     *         Если значение равно нулю, танк может произвести выстрел в данный тик.
     */
    public int getRemainingReloadingTime() {
        return remainingReloadingTime;
    }

    /**
     * @return Возвращает количество премиумных снарядов.
     */
    public int getPremiumShellCount() {
        return premiumShellCount;
    }

    /**
     * @return Возвращает {@code true} если и только если данный танк является дружественным.
     */
    public boolean isTeammate() {
        return teammate;
    }

    /**
     * @return Возвращает тип танка.
     */
    public TankType getType() {
        return type;
    }

    /**
     * @return Возвращает виртуальную длину орудия ---
     *         расстояние от центра танка до центра снаряда в момент выстрела.
     */
    public double getVirtualGunLength() {
        return type.getVirtualGunLength();
    }


    /**
     * @return Возвращает массу танка в единицах массы.
     */
    public double getMass() {
        return type.getMass();
    }

    /**
     * @return Возвращает мощность двигателя в единицах мощности.
     */
    public double getEnginePower() {
        return type.getEnginePower();
    }

    /**
     * @return Возвращает коэффициент мощности двигателя при движении назад.
     */
    public double getEngineRearPowerFactor() {
        return type.getEngineRearPowerFactor();
    }

    /**
     * @return Возвращает скорость поворота башни в радианах за тик.
     */
    public double getTurretTurnSpeed() {
        return type.getTurretTurnSpeed();
    }

    /**
     * @return Возвращает максимально возможный угол (в радианах) поворота башни относительно танка
     *         или {@code 0.0D}, если танк не имеет данного ограничения.
     */
    public double getTurretMaxRelativeAngle() {
        return type.getTurretMaxRelativeAngle();
    }

    /**
     * @return Возвращает максимальное здоровье экипажа.
     */
    public int getCrewMaxHealth() {
        return type.getCrewMaxHealth();
    }

    /**
     * @return Возвращает максимальную прочность корпуса.
     */
    public int getHullMaxDurability() {
        return type.getHullMaxDurability();
    }

    /**
     * @return Возвращает толщину лобовой брони танка в мм.
     */
    public int getFrontalArmor() {
        return type.getFrontalArmor();
    }

    /**
     * @return Возвращает толщину бортовой брони танка в мм.
     */
    public int getSideArmor() {
        return type.getSideArmor();
    }

    /**
     * @return Возвращает толщину кормовой брони танка в мм.
     */
    public int getRearArmor() {
        return type.getRearArmor();
    }

    /**
     * Returns the relative angle in range of {@code -PI} to {@code PI} both inclusive.
     *
     * @param x the abscissa coordinate of the point
     * @param y the ordinate coordinate of the point
     * @return angle to the point relative to this tank turret's angle
     */

    /**
     * @param x X-координата точки.
     * @param y Y-координата точки.
     * @return Возвращает ориентированный угол [{@code -PI}, {@code PI}] между
     *         орудием танка и вектором из центра танка (начала орудия) к указанной точке.
     */
    public double getTurretAngleTo(double x, double y) {
        double absoluteAngleTo = atan2(y - this.getY(), x - this.getX());
        double relativeAngleTo = absoluteAngleTo - getAngle() - turretRelativeAngle;

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
     * @param unit unit to calculate turret relative angle to
     * @return angle to the center of specified unit relative to this tank turret's angle
     */

    /**
     * @param unit Объект, к центру которого необходимо определить угол.
     * @return Возвращает ориентированный угол [{@code -PI}, {@code PI}] между
     *         орудием танка и вектором из центра танка (начала орудия) к центру указанного объекта.
     */
    public double getTurretAngleTo(Unit unit) {
        return getTurretAngleTo(unit.getX(), unit.getY());
    }
}
