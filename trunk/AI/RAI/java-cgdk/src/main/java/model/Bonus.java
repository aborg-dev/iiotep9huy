package model;

/**
 * Класс, определяющий бонус --- неподвижный полезный объект. Содержит также все свойства юнита.
 */
public final class Bonus extends Unit {
    private final BonusType type;

    public Bonus(long id, double width, double height, double x, double y, BonusType type) {
        super(id, width, height, x, y, 0.0D, 0.0D, 0.0D, 0.0D);
        this.type = type;
    }

    /**
     * @return Возвращает тип бонуса.
     */
    public BonusType getType() {
        return type;
    }
}
