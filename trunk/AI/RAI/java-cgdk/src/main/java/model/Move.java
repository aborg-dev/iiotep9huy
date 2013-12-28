package model;

/**
 * Стратегия игрока может управлять танком посредством установки свойств объекта данного класса.
 */
public final class Move {
    /**
     * Should be from {@code -1.0D} to {@code 1.0D}.
     */
    private double leftTrackPower;

    /**
     * Should be from {@code -1.0D} to {@code 1.0D}.
     */
    private double rightTrackPower;

    private double turretTurn;

    private FireType fireType;

    /**
     * @return Возвращает текущую установку мощности двигателя для левой гусеницы.
     */
    public double getLeftTrackPower() {
        return leftTrackPower;
    }

    /**
     * Задаёт мощность двигателя для левой гусеницы.
     *
     * @param leftTrackPower Значение в интервале [{@code -1.0}, {@code 1.0}],
     *                       где {@code -1.0} означает <<полный назад>>,
     *                       а {@code 1.0} --- <<полный вперёд>>.
     *                       Значения, лежащие за пределами интервала, будут приведены к ближайшей его границе.
     */
    public void setLeftTrackPower(double leftTrackPower) {
        this.leftTrackPower = leftTrackPower;
    }

    /**
     * @return Возвращает текущую установку мощности двигателя для правой гусеницы.
     */
    public double getRightTrackPower() {
        return rightTrackPower;
    }

    /**
     * Задаёт мощность двигателя для правой гусеницы.
     *
     * @param rightTrackPower Значение в интервале [{@code -1.0}, {@code 1.0}],
     *                        где {@code -1.0} означает <<полный назад>>,
     *                        а {@code 1.0} --- <<полный вперёд>>.
     *                        Значения, лежащие за пределами интервала, будут приведены к ближайшей его границе.
     */
    public void setRightTrackPower(double rightTrackPower) {
        this.rightTrackPower = rightTrackPower;
    }

    /**
     * @return Возвращает текущую установку для поворота башни.
     */
    public double getTurretTurn() {
        return turretTurn;
    }

    /**
     * Задаёт направление и скорость поворота башни в следующий тик.
     *
     * @param turretTurn Значение в интервале [{@code -a}, {@code a}],
     *                   где {@code a} --- табличное значение скорости поворота башни для данного типа танка.
     *                   Положительные значения соответствуют повороту по часовой стрелке,
     *                   отрицательные --- против.
     *                   Значения, лежащие за пределами интервала, будут приведены к ближайшей его границе.
     */
    public void setTurretTurn(double turretTurn) {
        this.turretTurn = turretTurn;
    }

    /**
     * @return Возвращает текущий режим стрельбы.
     */
    public FireType getFireType() {
        return fireType;
    }

    /**
     * Устанавливает режим стрельбы.
     *
     * @param fireType Режим стрельбы.
     */
    public void setFireType(FireType fireType) {
        this.fireType = fireType;
    }
}
