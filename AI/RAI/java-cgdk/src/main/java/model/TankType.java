package model;

import static java.lang.StrictMath.PI;

/**
 * Тип танка.
 */
public enum TankType {
    /**
     * Средний танк, СТ.
     */
    MEDIUM(90.0D, 60.0D, 67.5D, 10.0D, 7500.0D, 0.75D, 1.0D * PI / 180.0D, 0.0D * PI / 180.0D, 100, 200, 175, 150, 100),

    /**
     * Тяжёлый танк, ТТ.
     */
    HEAVY(105.0D, 75.0D, 82.5D, 20.0D, 7500.0D, 0.5D, 0.5D * PI / 180.0D, 0.0D * PI / 180.0D, 100, 250, 200, 175, 100),

    /**
     * Самоходное противотанковое орудие, ПТ.
     */
    TANK_DESTROYER(
            112.5D, 67.5D, 97.5D, 15.0D, 5000.0D, 0.35D,
            1.5D * PI / 180.0D, 15.0D * PI / 180.0D, 100, 250, 250, 125, 100
    );

    private final double width;
    private final double height;
    private final double virtualGunLength;
    private final double mass;
    private final double enginePower;
    private final double engineRearPowerFactor;
    private final double turretTurnSpeed;
    private final double turretMaxRelativeAngle;
    private final int crewMaxHealth;
    private final int hullMaxDurability;
    private final int frontalArmor;
    private final int sideArmor;
    private final int rearArmor;

    TankType(
            double width, double height, double virtualGunLength,
            double mass, double enginePower, double engineRearPowerFactor,
            double turretTurnSpeed, double turretMaxRelativeAngle, int crewMaxHealth, int hullMaxDurability,
            int frontalArmor, int sideArmor, int rearArmor) {
        this.width = width;
        this.height = height;
        this.virtualGunLength = virtualGunLength;
        this.mass = mass;
        this.enginePower = enginePower;
        this.engineRearPowerFactor = engineRearPowerFactor;
        this.turretTurnSpeed = turretTurnSpeed;
        this.turretMaxRelativeAngle = turretMaxRelativeAngle;
        this.crewMaxHealth = crewMaxHealth;
        this.hullMaxDurability = hullMaxDurability;
        this.frontalArmor = frontalArmor;
        this.sideArmor = sideArmor;
        this.rearArmor = rearArmor;
    }

    double getWidth() {
        return width;
    }

    double getHeight() {
        return height;
    }

    double getVirtualGunLength() {
        return virtualGunLength;
    }

    double getMass() {
        return mass;
    }

    double getEnginePower() {
        return enginePower;
    }

    double getEngineRearPowerFactor() {
        return engineRearPowerFactor;
    }

    double getTurretTurnSpeed() {
        return turretTurnSpeed;
    }

    double getTurretMaxRelativeAngle() {
        return turretMaxRelativeAngle;
    }

    int getCrewMaxHealth() {
        return crewMaxHealth;
    }

    int getHullMaxDurability() {
        return hullMaxDurability;
    }

    int getFrontalArmor() {
        return frontalArmor;
    }

    int getSideArmor() {
        return sideArmor;
    }

    int getRearArmor() {
        return rearArmor;
    }
}
