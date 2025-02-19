# 🕹️ JoyTracker

JoyTracker é um programa desenvolvido em **C11** para a placa **BitDogLab**, que mapeia o movimento de um **joystick** e exibe sua posição em um **display OLED SSD1306 (128x64)**, além de controlar **LEDs RGB** com base nos valores do joystick e dos botões físicos.

## 📌 Índice

1. [📖 Descrição do Projeto](#descrição-do-projeto)
2. [🛠 Componentes Utilizados](#componentes-utilizados)
3. [📌 Requisitos do Projeto](#requisitos-do-projeto)
4. [🚀 Tecnologias Utilizadas](#tecnologias-utilizadas)
5. [🔧 Como Compilar e Executar](#como-compilar-e-executar)
6. [📜 Estrutura do Projeto](#estrutura-do-projeto)
7. [🔥 Melhorias Futuras](#melhorias-futuras)
8. [📌 Licença](#licença)

<a id="descrição-do-projeto"></a>
## 📖 Descrição do Projeto

O **joystick** fornece valores analógicos correspondentes aos eixos **X** e **Y**, que são utilizados para:

- **🎨 Controlar a intensidade luminosa dos LEDs RGB:**
  - **🔵 LED Azul:** Brilho ajustado conforme o eixo **Y**. Quando o joystick estiver na posição central (**2048**), o LED estará apagado. Ao mover o joystick para **cima** (valores menores) ou **baixo** (valores maiores), o brilho aumenta gradualmente, atingindo o máximo nos extremos (**0 e 4095**).
  - **🔴 LED Vermelho:** Segue o mesmo princípio, mas baseado no eixo **X**.
  - **🌈 Os LEDs são controlados via PWM**, permitindo variação suave da intensidade luminosa.

- **🖥️ Exibir um quadrado de 8x8 pixels no display SSD1306:**
  - Inicialmente **centralizado**.
  - Se movimenta proporcionalmente aos valores capturados pelo **joystick**.

- **🎮 Controlar ações do botão do joystick:**
  - Alternar o estado do **🟢 LED Verde** a cada pressionamento.
  - Modificar a borda do display para indicar o pressionamento do botão, alternando entre diferentes estilos de borda.

- **🕹️ Controlar ações do botão A:**
  - Ativar ou desativar os **LEDs PWM** a cada acionamento.

<a id="componentes-utilizados"></a>
## 🛠 Componentes Utilizados

Os seguintes componentes já estão **interconectados** à placa **BitDogLab**:

| 🏷️ Componente  | 📌 GPIO |
|-------------|------|
| **💡 LED RGB** (Vermelho, Azul, Verde) | 11, 12, 13 |
| **🔘 Botão do Joystick** | 22 |
| **🕹️ Joystick (Eixo X, Y)** | 26, 27 |
| **🅰️ Botão A** | 5 |
| **🖥️ Display SSD1306 (I2C)** | 14, 15 |

<a id="requisitos-do-projeto"></a>
## 📌 Requisitos do Projeto

O desenvolvimento do **JoyTracker** seguiu os seguintes requisitos:

1. **🛑 Uso de Interrupções:** Todas as funcionalidades relacionadas aos **botões** são implementadas utilizando **rotinas de interrupção (IRQ)**.
2. **🔄 Debouncing:** Implementação de tratamento de **bouncing** dos botões via software.
3. **🖥️ Utilização do Display OLED 128x64:** Explora ferramentas gráficas e protocolo **I2C** para exibição de elementos visuais.
4. **📄 Organização do Código:** Código **estruturado e comentado** para facilitar o entendimento.

<a id="tecnologias-utilizadas"></a>
## 🚀 Tecnologias Utilizadas

- **💻 Linguagem:** C11
- **📦 SDK:** pico-sdk versão 2.1.0
- **🛠️ Ambiente de Desenvolvimento:** VS Code
- **📝 Build System:** CMake

<a id="como-compilar-e-executar"></a>
## 🔧 Como Compilar e Executar

### 🔹 Pré-requisitos

Certifique-se de ter instalado:
- **📌 Raspberry Pi Pico SDK**
- **🛠️ CMake**
- **🖥️ Compilador ARM GCC (gcc-arm-none-eabi)**
- **📡 Bibliotecas para comunicação I2C e controle de display OLED**

### 🔹 Compilação

1. Clone este repositório:
   ```sh
   git clone https://github.com/seuusuario/JoyTracker.git
   cd JoyTracker
   ```
2. Crie um diretório `build` e entre nele:
   ```sh
   mkdir build && cd build
   ```
3. Execute o CMake:
   ```sh
   cmake ..
   ```
4. Compile o projeto:
   ```sh
   make
   ```

### 🔹 Upload para a placa

Após a compilação, conecte sua **Raspberry Pi Pico** ao computador em **modo bootloader**, e copie o arquivo `.uf2` gerado para o dispositivo correspondente.

<a id="estrutura-do-projeto"></a>
## 📜 Estrutura do Projeto

```
JoyTracker/
│── src/               # Código-fonte principal
│   ├── main.c         # Implementação principal do JoyTracker
│   ├── joystick.h     # Cabeçalho para manipulação do joystick
│   ├── joygfx.h       # Cabeçalho para controle gráfico do display
│   ├── pwm_led.h      # Cabeçalho para controle dos LEDs via PWM
│── CMakeLists.txt     # Configuração do CMake
│── README.md          # Documentação do projeto
```

<a id="melhorias-futuras"></a>
## 🔥 Melhorias Futuras

- 🏁 Implementação de um **histórico de rastreamento** para mostrar o caminho percorrido pelo joystick no display.
- 🎨 Suporte para **diferentes padrões de movimento** do cursor no display.
- 📜 Criação de um **menu interativo** no display OLED.

<a id="licença"></a>
## 📌 Licença

Este projeto está sob a **MIT License** - sinta-se livre para utilizá-lo e modificá-lo conforme necessário! 🛠🚀

---

📌 **Criado por:** [Carlos Valadão]  
📧 Contato: [chovaladao@gmail.com]  
🔗 GitHub: [github.com/CarlosValadao]  
