import model.*;

import static java.lang.StrictMath.PI;

public final class MyStrategy implements Strategy {
    @Override
    public void move(Tank self, World world, Move move) {
        move.setLeftTrackPower(-1.0D);
        move.setRightTrackPower(1.0D);
        move.setTurretTurn(PI);
        move.setFireType(FireType.PREMIUM_PREFERRED);
    }

    @Override
    public TankType selectTank(int tankIndex, int teamSize) {
        return TankType.MEDIUM;
    }
}
