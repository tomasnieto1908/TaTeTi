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

while True:
    try:
        # Leer datos del puerto serie
        if ser.in_waiting > 0:
            data = ser.readline().decode().strip()
            print(f"Datos recibidos: {data}")  # Depuración para ver qué se está recibiendo

            # Verificar si el formato es el esperado
            if 'mov: ' in data:
                partes = data.split('mov: ')
                
                if len(partes) == 2:  # Asegurarse de que hay dos partes para desempaquetar
                    ID_Jugador, movimiento_data = partes
                    movimiento_partes = movimiento_data.split(':')

                    if len(movimiento_partes) == 2:  # Verificar si hay dos partes (ID_Ficha y Posicion)
                        ID_Ficha, posicion = movimiento_partes
                        
                        if posicion.isdigit():  # Asegurarse de que la posición sea un número
                            Posicion = int(posicion)

                            # Obtener el timestamp actual
                            timestamp = datetime.now().strftime('%Y-%m-%d %H:%M:%S')

                            # Insertar datos en la base de datos con ID_Jugador, ID_Ficha y la posición final
                            c.execute('''
                                INSERT INTO Movimientos (ID_Jugador, ID_Ficha, Posicion, Timestamp)
                                VALUES (?, ?, ?, ?)
                            ''', (ID_Jugador, ID_Ficha, Posicion, timestamp))

                            # Confirmar los cambios
                            conn.commit()
                            print(f"Datos almacenados: Jugador: {ID_Jugador}, Ficha: {ID_Ficha}, Posición Final: {Posicion}, Timestamp: {timestamp}")
                        else:
                            print(f"Error: La posición no es un número válido: {posicion}")
                    else:
                        print(f"Error: Formato de movimiento incorrecto: {movimiento_data}")
                else:
                    print(f"Error: Formato de jugador incorrecto: {data}")
            else:
                print(f"Advertencia: El formato no contiene 'mov: ', datos ignorados.")
    
    except KeyboardInterrupt:
        print("Interrupción del teclado. Cerrando...")
        break
    except Exception as e:
        print(f"Error inesperado: {e}")

# Cerrar conexiones
ser.close()
conn.close()
