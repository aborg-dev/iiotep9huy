package model;

import java.util.Arrays;

/**
 * Содержит описание игрового мира и позволяет получить списки различных юнитов, присутствующих на поле боя.
 */
public final class World {
    private final int tick;
    private final double width;
    private final double height;
    private final Player[] players;
    private final Obstacle[] obstacles;
    private final Tank[] tanks;
    private final Shell[] shells;
    private final Bonus[] bonuses;

    public World(int tick, double width, double height, Player[] players,
                 Obstacle[] obstacles, Tank[] tanks, Shell[] shells, Bonus[] bonuses) {
        this.tick = tick;
        this.width = width;
        this.height = height;
        this.players = Arrays.copyOf(players, players.length);
        this.obstacles = Arrays.copyOf(obstacles, obstacles.length);
        this.tanks = Arrays.copyOf(tanks, tanks.length);
        this.shells = Arrays.copyOf(shells, shells.length);
        this.bonuses = Arrays.copyOf(bonuses, bonuses.length);
    }

    /**
     * @return Возвращает номер текущего тика.
     */
    public int getTick() {
        return tick;
    }

    /**
     * @return Возвращает ширину мира в точках.
     */
    public double getWidth() {
        return width;
    }

    /**
     * @return Возвращает высоту мира в точках.
     */
    public double getHeight() {
        return height;
    }

    /**
     * @return Возвращает список игроков (в случайном порядке).
     *         После каждого тика объекты, задающие игроков, пересоздаются.
     */
    public Player[] getPlayers() {
        return Arrays.copyOf(players, players.length);
    }

    /**
     * @return Возвращает список препятствий (в случайном порядке).
     *         После каждого тика объекты, задающие препятствия, пересоздаются.
     */
    public Obstacle[] getObstacles() {
        return Arrays.copyOf(obstacles, obstacles.length);
    }

    /**
     * @return Возвращает список танков (в случайном порядке), включая танк стратегии, вызвавшей этот метод.
     *         После каждого тика объекты, задающие танки, пересоздаются.
     */
    public Tank[] getTanks() {
        return Arrays.copyOf(tanks, tanks.length);
    }

    /**
     * @return Возвращает список снарядов (в случайном порядке).
     *         После каждого тика объекты, задающие снаряды, пересоздаются.
     */
    public Shell[] getShells() {
        return Arrays.copyOf(shells, shells.length);
    }

    /**
     * @return Возвращает список бонусов (в случайном порядке).
     *         После каждого тика объекты, задающие бонусы, пересоздаются.
     */
    public Bonus[] getBonuses() {
        return Arrays.copyOf(bonuses, bonuses.length);
    }
}
