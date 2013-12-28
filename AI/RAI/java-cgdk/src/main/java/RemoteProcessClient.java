import model.*;

import java.io.*;
import java.net.Socket;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;

public final class RemoteProcessClient implements Closeable {
    private static final int BUFFER_SIZE_BYTES = 1 << 20;
    private static final ByteOrder PROTOCOL_BYTE_ORDER = ByteOrder.LITTLE_ENDIAN;
    private static final int INTEGER_SIZE_BYTES = Integer.SIZE / Byte.SIZE;
    private static final int LONG_SIZE_BYTES = Long.SIZE / Byte.SIZE;

    private final Socket socket;
    private final InputStream inputStream;
    private final OutputStream outputStream;
    private final ByteArrayOutputStream outputStreamBuffer;

    public RemoteProcessClient(String host, int port) throws IOException {
        socket = new Socket(host, port);
        socket.setSendBufferSize(BUFFER_SIZE_BYTES);
        socket.setReceiveBufferSize(BUFFER_SIZE_BYTES);

        inputStream = socket.getInputStream();
        outputStream = socket.getOutputStream();
        outputStreamBuffer = new ByteArrayOutputStream(BUFFER_SIZE_BYTES);
    }

    public void writeToken(String token) throws IOException {
        writeEnum(MessageType.AUTHENTICATION_TOKEN);
        writeString(token);
        flush();
    }

    public int readTeamSize() throws IOException {
        ensureMessageType(readEnum(MessageType.class), MessageType.TEAM_SIZE);
        return readInt();
    }

    public void writeSelectedTanks(TankType[] tankTypes) throws IOException {
        writeEnum(MessageType.TANK_TYPES);

        if (tankTypes == null) {
            writeInt(-1);
        } else {
            int typeCount = tankTypes.length;
            writeInt(typeCount);

            for (int typeIndex = 0; typeIndex < typeCount; ++typeIndex) {
                writeEnum(tankTypes[typeIndex]);
            }
        }

        flush();
    }

    public PlayerContext readPlayerContext() throws IOException {
        MessageType messageType = readEnum(MessageType.class);
        if (messageType == MessageType.GAME_OVER) {
            return null;
        }

        ensureMessageType(messageType, MessageType.PLAYER_CONTEXT);
        return readBoolean() ? new PlayerContext(readTanks(), readWorld()) : null;
    }

    public void writeMoves(Move[] moves) throws IOException {
        writeEnum(MessageType.MOVES);

        if (moves == null) {
            writeInt(-1);
        } else {
            int moveCount = moves.length;
            writeInt(moveCount);

            for (int moveIndex = 0; moveIndex < moveCount; ++moveIndex) {
                Move move = moves[moveIndex];

                if (move == null) {
                    writeBoolean(false);
                } else {
                    writeBoolean(true);

                    writeDouble(move.getLeftTrackPower());
                    writeDouble(move.getRightTrackPower());
                    writeDouble(move.getTurretTurn());
                    writeEnum(move.getFireType());
                }
            }
        }

        flush();
    }

    @Override
    public void close() throws IOException {
        socket.close();
    }

    private World readWorld() throws IOException {
        if (!readBoolean()) {
            return null;
        }

        return new World(
                readInt(), readDouble(), readDouble(),
                readPlayers(), readObstacles(), readTanks(), readShells(), readBonuses()
        );
    }

    private Player[] readPlayers() throws IOException {
        int playerCount = readInt();
        if (playerCount < 0) {
            return null;
        }

        Player[] players = new Player[playerCount];

        for (int playerIndex = 0; playerIndex < playerCount; ++playerIndex) {
            if (readBoolean()) {
                players[playerIndex] = new Player(readString(), readInt(), readBoolean());
            }
        }

        return players;
    }

    private Obstacle[] readObstacles() throws IOException {
        int obstacleCount = readInt();
        if (obstacleCount < 0) {
            return null;
        }

        Obstacle[] obstacles = new Obstacle[obstacleCount];

        for (int obstacleIndex = 0; obstacleIndex < obstacleCount; ++obstacleIndex) {
            if (readBoolean()) {
                obstacles[obstacleIndex] = new Obstacle(
                        readLong(), readDouble(), readDouble(), readDouble(), readDouble()
                );
            }
        }

        return obstacles;
    }

    private Tank[] readTanks() throws IOException {
        int tankCount = readInt();
        if (tankCount < 0) {
            return null;
        }

        Tank[] tanks = new Tank[tankCount];

        for (int tankIndex = 0; tankIndex < tankCount; ++tankIndex) {
            if (readBoolean()) {
                tanks[tankIndex] = new Tank(
                        readLong(), readString(), readInt(), readDouble(), readDouble(),
                        readDouble(), readDouble(), readDouble(), readDouble(), readDouble(),
                        readInt(), readInt(), readInt(), readInt(), readInt(),
                        readBoolean(), readEnum(TankType.class)
                );
            }
        }

        return tanks;
    }

    private Shell[] readShells() throws IOException {
        int shellCount = readInt();
        if (shellCount < 0) {
            return null;
        }

        Shell[] shells = new Shell[shellCount];

        for (int shellIndex = 0; shellIndex < shellCount; ++shellIndex) {
            if (readBoolean()) {
                shells[shellIndex] = new Shell(
                        readLong(), readString(), readDouble(), readDouble(), readDouble(), readDouble(),
                        readDouble(), readDouble(), readDouble(), readDouble(), readEnum(ShellType.class)
                );
            }
        }

        return shells;
    }

    private Bonus[] readBonuses() throws IOException {
        int bonusCount = readInt();
        if (bonusCount < 0) {
            return null;
        }

        Bonus[] bonuses = new Bonus[bonusCount];

        for (int bonusIndex = 0; bonusIndex < bonusCount; ++bonusIndex) {
            if (readBoolean()) {
                bonuses[bonusIndex] = new Bonus(
                        readLong(), readDouble(), readDouble(), readDouble(), readDouble(), readEnum(BonusType.class)
                );
            }
        }

        return bonuses;
    }

    private static void ensureMessageType(MessageType actualType, MessageType expectedType) {
        if (actualType != expectedType) {
            throw new IllegalArgumentException(String.format(
                    "Received wrong message [actual=%s, expected=%s].", actualType, expectedType
            ));
        }
    }

    private <E extends Enum> E readEnum(Class<E> enumClass) throws IOException {
        byte ordinal = readBytes(1)[0];

        E[] values = enumClass.getEnumConstants();
        int valueCount = values.length;

        for (int valueIndex = 0; valueIndex < valueCount; ++valueIndex) {
            E value = values[valueIndex];
            if (value.ordinal() == ordinal) {
                return value;
            }
        }

        return null;
    }

    private <E extends Enum> void writeEnum(E value) throws IOException {
        writeBytes(new byte[]{(byte) (value == null ? -1 : value.ordinal())});
    }

    private String readString() throws IOException {
        int length = readInt();
        if (length == -1) {
            return null;
        }

        try {
            return new String(readBytes(length), "UTF-8");
        } catch (UnsupportedEncodingException e) {
            throw new IllegalArgumentException("UTF-8 is unsupported.", e);
        }
    }

    private void writeString(String value) throws IOException {
        if (value == null) {
            writeInt(-1);
            return;
        }

        byte[] bytes;
        try {
            bytes = value.getBytes("UTF-8");
        } catch (UnsupportedEncodingException e) {
            throw new IllegalArgumentException("UTF-8 is unsupported.", e);
        }

        writeInt(bytes.length);
        writeBytes(bytes);
    }

    private boolean readBoolean() throws IOException {
        return readBytes(1)[0] == 1;
    }

    private void writeBoolean(boolean value) throws IOException {
        writeBytes(new byte[]{(byte) (value ? 1 : 0)});
    }

    private int readInt() throws IOException {
        return ByteBuffer.wrap(readBytes(INTEGER_SIZE_BYTES)).order(PROTOCOL_BYTE_ORDER).getInt();
    }

    private void writeInt(int value) throws IOException {
        writeBytes(ByteBuffer.allocate(INTEGER_SIZE_BYTES).order(PROTOCOL_BYTE_ORDER).putInt(value).array());
    }

    private long readLong() throws IOException {
        return ByteBuffer.wrap(readBytes(LONG_SIZE_BYTES)).order(PROTOCOL_BYTE_ORDER).getLong();
    }

    private void writeLong(long value) throws IOException {
        writeBytes(ByteBuffer.allocate(LONG_SIZE_BYTES).order(PROTOCOL_BYTE_ORDER).putLong(value).array());
    }

    private double readDouble() throws IOException {
        return Double.longBitsToDouble(readLong());
    }

    private void writeDouble(double value) throws IOException {
        writeLong(Double.doubleToLongBits(value));
    }

    private byte[] readBytes(int byteCount) throws IOException {
        byte[] bytes = new byte[byteCount];
        int offset = 0;
        int readByteCount;

        while (offset < byteCount && (readByteCount = inputStream.read(bytes, offset, byteCount - offset)) != -1) {
            offset += readByteCount;
        }

        if (offset != byteCount) {
            throw new IOException(String.format("Can't read %d bytes from input stream.", byteCount));
        }

        return bytes;
    }

    private void writeBytes(byte[] bytes) throws IOException {
        outputStreamBuffer.write(bytes);
    }

    private void flush() throws IOException {
        outputStream.write(outputStreamBuffer.toByteArray());
        outputStreamBuffer.reset();
        outputStream.flush();
    }

    private enum MessageType {
        UNKNOWN,
        GAME_OVER,
        AUTHENTICATION_TOKEN,
        TEAM_SIZE,
        TANK_TYPES,
        PLAYER_CONTEXT,
        MOVES
    }
}
