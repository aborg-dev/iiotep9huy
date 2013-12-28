package model;

/**
 * Класс, определяющий снаряд. Содержит также все свойства юнита.
 */
public final class Shell extends Unit {
    private final String playerName;
    private final ShellType type;

    public Shell(
            long id, String playerName, double width, double height, double x, double y,
            double speedX, double speedY, double angle, double angularSpeed, ShellType type) {
        super(id, width, height, x, y, speedX, speedY, angle, angularSpeed);

        this.playerName = playerName;
        this.type = type;
    }

    /**
     * @return Возвращает имя игрока --- владельца танка, выпустившего снаряд.
     */
    public String getPlayerName() {
        return playerName;
    }

    /**
     * @return Возвращает тип снаряда.
     */
    public ShellType getType() {
        return type;
    }
}
