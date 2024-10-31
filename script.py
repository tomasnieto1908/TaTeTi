import serial
import sqlite3
from datetime import datetime

# Configuración del puerto serie
puerto_serial = '/dev/ttyACM0'  # Cambia esto según tu sistema
baud_rate = 9600

# Conectar al puerto serie
ser = serial.Serial(puerto_serial, baud_rate, timeout=1)

# Conectar a la base de datos SQLite
conn = sqlite3.connect('tateti.db')
c = conn.cursor()

# Variable para llevar el conteo de partidas
id_partida = 0

# Variable para marcar si la partida ha terminado
partida_finalizada = False

while True:
    try:
        # Leer datos del puerto serie
        if ser.in_waiting > 0:
            data = ser.readline().decode().strip()
            print(f"Datos recibidos: {data}")

            # Verificar si el mensaje es "fin del juego" y la partida no ha sido marcada como finalizada
            if data == "FIN de JUEGO" and not partida_finalizada:
                id_partida += 1  # Incrementar el ID de partida
                partida_finalizada = True  # Marcar la partida como finalizada
                print(f"Fin del juego detectado. Incrementando ID_Partida a {id_partida}. Esperando una nueva partida.")

            # Procesar los movimientos de una nueva partida si el formato es correcto
            elif 'mov: ' in data:
                # Marcar que la nueva partida ha comenzado
                partida_finalizada = False

                partes = data.split('mov: ')

                if len(partes) == 2:
                    ID_Jugador, movimiento_data = partes
                    movimiento_partes = movimiento_data.split(',')

                    if len(movimiento_partes) == 2:
                        ID_Ficha, posicion = movimiento_partes

                        if posicion.isdigit():
                            Posicion = int(posicion)
                            timestamp = datetime.now().strftime('%Y-%m-%d %H:%M:%S')

                            # Determinar el ID_Jugador
                            jugador_id = 0 if ID_Ficha[0] == 'R' else 1

                            # Insertar datos en la base de datos
                            c.execute('''
                                INSERT INTO Movimientos (ID_Jugador, ID_Ficha, Posicion, Timestamp, ID_Partida)
                                VALUES (?, ?, ?, ?, ?)
                            ''', (jugador_id, ID_Ficha, Posicion, timestamp, id_partida))

                            conn.commit()
                            print(f"Datos almacenados: Jugador: {jugador_id}, Ficha: {ID_Ficha}, Posición: {Posicion}, Timestamp: {timestamp}, ID_Partida: {id_partida}")
                        else:
                            print(f"Error: La posición no es un número válido: {posicion}")
                    else:
                        print(f"Error: Formato de movimiento incorrecto: {movimiento_data}")
                else:
                    print(f"Error: Formato de jugador incorrecto: {data}")
            else:
                print(f"Advertencia: Formato desconocido: {data}")

    except KeyboardInterrupt:
        print("Interrupción del teclado. Cerrando...")
        break
    except Exception as e:
        print(f"Error inesperado: {e}")

# Cerrar conexiones
ser.close()
conn.close()
