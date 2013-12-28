package model;

/**
 * Содержит данные о текущем состоянии игрока.
 */
public final class Player {
    private final String name;
    private final int score;
    private final boolean strategyCrashed;

    public Player(String name, int score, boolean strategyCrashed) {
        this.name = name;
        this.score = score;
        this.strategyCrashed = strategyCrashed;
    }

    /**
     * @return Возвращает имя игрока.
     */
    public String getName() {
        return name;
    }

    /**
     * @return Возвращает текущее количество баллов, набранных игроком.
     */
    public int getScore() {
        return score;
    }

    /**
     * @return Возвращает специальный флаг --- показатель того, что стратегия игрока <<упала>>.
     *         Более подробную информацию можно найти в документации к игре.
     */
    public boolean isStrategyCrashed() {
        return strategyCrashed;
    }
}
