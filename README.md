# Guia Generica de Compilacion y Ejecucion Manual (C++)

Esta guia contiene las plantillas e instrucciones genericas para compilar y ejecutar cualquier archivo C++ en Windows (PowerShell) dentro de este entorno.

---

## 1. Verificacion del Entorno

Comprobar que el compilador `g++` este disponible en la terminal actual:

```powershell
g++ --version
```

### Recargar variable PATH si el comando no es reconocido
Si PowerShell muestra el error `The term 'g++' is not recognized`, ejecute el siguiente comando para actualizar el `PATH` de la sesion actual sin reiniciar:

```powershell
$env:Path = [System.Environment]::GetEnvironmentVariable("Path","Machine") + ";" + [System.Environment]::GetEnvironmentVariable("Path","User")
```

---

## 2. Comandos Genericos de Compilacion

Reemplace `<archivo>` por el nombre base de su archivo de codigo (por ejemplo, si el archivo es `main.cpp`, use `main`).

### Metodo A: Estandar (Dentro de la carpeta donde reside el codigo)

1. **Compilacion optimizada (Recomendada para programacion competitiva)**:
   ```powershell
   g++ -O3 -std=c++17 -static <archivo>.cpp -o <archivo>.exe
   ```
   > **Nota sobre `-static`**: Empaqueta el runtime de C++ dentro del binario para que el ejecutable funcione de forma autonoma sin requerir dependencias externas de DLLs.

2. **Compilacion en modo depuracion (Warnings y sanitizers)**:
   ```powershell
   g++ -std=c++17 -Wall -Wextra -Wshadow -fsanitize=undefined <archivo>.cpp -o <archivo>.exe
   ```

### Metodo B: Desde la carpeta raiz del proyecto

Si se encuentra en la raiz del espacio de trabajo y el archivo esta en una subcarpeta:

```powershell
g++ -O3 -std=c++17 -static "<subcarpeta>\<archivo>.cpp" -o "<subcarpeta>\<archivo>.exe"
```

---

## 3. Comandos Genericos de Ejecucion

### Opcion 1: Interactiva (Entrada manual por teclado)

```powershell
.\<archivo>.exe
```
- Para senalar fin de entrada (EOF) en consola de Windows: presione `Ctrl + Z` seguido de `Enter`.

### Opcion 2: Redireccion desde un archivo de entrada

Cuando se tienen casos de prueba almacenados en un archivo de texto (por ejemplo, `input.txt`):

```powershell
Get-Content <archivo_entrada>.txt | .\<archivo>.exe
```

O usando el interprete `cmd`:

```powershell
cmd /c "<archivo>.exe < <archivo_entrada>.txt"
```

### Opcion 3: Redireccion de entrada y salida a archivos

Para capturar la salida en un archivo (por ejemplo, `output.txt`):

```powershell
Get-Content <archivo_entrada>.txt | .\<archivo>.exe > <archivo_salida>.txt
```

O con `cmd`:

```powershell
cmd /c "<archivo>.exe < <archivo_entrada>.txt > <archivo_salida>.txt"
```

### Opcion 4: Envio directo de datos por tuberia (Pipe)

Para probar una entrada especifica sin crear archivos intermedios:

```powershell
"<datos_de_entrada>" | .\<archivo>.exe
```
*(Nota: En PowerShell, use `` `n `` para representar saltos de linea dentro del texto).*

---

## 4. Ejemplo Concreto

Para un problema ubicado en `2260B` con archivo fuente `2260B.cpp`:

1. Navegar a la carpeta:
   ```powershell
   cd 2260B
   ```
2. Compilar:
   ```powershell
   g++ -O3 -std=c++17 -static 2260B.cpp -o 2260B.exe
   ```
3. Ejecutar con una entrada directa de prueba:
   ```powershell
   "1`n5 10 3" | .\2260B.exe
   ```

---

## 5. Referencia de Banderas

| Bandera | Funcion |
| :--- | :--- |
| `-std=c++17` | Habilita el estandar C++17. |
| `-O3` | Aplica optimizaciones de rendimiento en tiempo de ejecucion. |
| `-static` | Enlaza estaticamente librerias del sistema para evitar fallos por DLLs ausentes. |
| `-Wall -Wextra` | Emite advertencias sobre posibles errores o inconsistencias en el codigo. |
| `-Wshadow` | Advierte cuando una variable local oculta a otra de ambito superior. |
| `-fsanitize=undefined` | Detecta desbordamientos y comportamientos indefinidos durante la ejecucion de pruebas. |
