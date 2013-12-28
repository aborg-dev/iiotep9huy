package model;

import java.util.Arrays;

public final class PlayerContext {
    private final Tank[] tanks;
    private final World world;

    public PlayerContext(Tank[] tanks, World world) {
        this.tanks = Arrays.copyOf(tanks, tanks.length);
        this.world = world;
    }

    public Tank[] getTanks() {
        return Arrays.copyOf(tanks, tanks.length);
    }

    public World getWorld() {
        return world;
    }
}
