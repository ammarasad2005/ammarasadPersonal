TITLE Space Shooter Game - x86 MASM with Irvine Library
; This program implements a space shooter game using x86 MASM and the Irvine library
; Author: Generated for ammarasadPersonal
; Date: 2024

INCLUDE Irvine32.inc

.data
    ; Game constants
    SCREEN_WIDTH = 80
    SCREEN_HEIGHT = 22    ; Leave space for UI
    MAX_ENEMIES = 10
    MAX_BULLETS = 20
    PLAYER_START_X = 40
    PLAYER_START_Y = 20
    ENEMY_SPAWN_RATE = 8  ; Higher number = less frequent spawning
    
    ; Game variables
    playerX DWORD PLAYER_START_X
    playerY DWORD PLAYER_START_Y
    playerChar BYTE '*'
    
    score DWORD 0
    gameRunning DWORD 1
    
    ; Enemy array structure: [x, y, active, moveCounter]
    enemies DWORD MAX_ENEMIES * 4 DUP(0)  ; x, y, active, moveCounter for each enemy
    enemyChar BYTE 'V'
    
    ; Bullet array structure: [x, y, active, direction]
    bullets DWORD MAX_BULLETS * 4 DUP(0)  ; x, y, active, direction for each bullet
    bulletChar BYTE '|'
    
    ; Screen buffer
    screenBuffer BYTE SCREEN_WIDTH * SCREEN_HEIGHT DUP(' ')
    
    ; Game messages
    titleMsg BYTE "=== SPACE SHOOTER ===", 0
    scoreMsg BYTE "Score: ", 0
    gameOverMsg BYTE "GAME OVER! Final Score: ", 0
    instructionsMsg BYTE "Controls: A/D - Move, SPACE - Shoot, Q - Quit", 0
    
    ; Input handling
    inputChar BYTE ?
    
.code
main PROC
    ; Initialize game
    call InitializeGame
    
    ; Main game loop
    GameLoop:
        cmp gameRunning, 0
        je GameEnd
        
        ; Clear screen buffer
        call ClearScreenBuffer
        
        ; Handle input
        call HandleInput
        
        ; Update game objects
        call UpdatePlayer
        call UpdateEnemies
        call UpdateBullets
        
        ; Check collisions
        call CheckCollisions
        
        ; Spawn new enemies occasionally
        call SpawnEnemies
        
        ; Render game
        call RenderGame
        
        ; Display screen
        call DisplayScreen
        
        ; Small delay
        mov eax, 50
        call Delay
        
        jmp GameLoop
    
    GameEnd:
        call DisplayGameOver
        
    exit
main ENDP

; Initialize game state
InitializeGame PROC
    ; Clear console
    call Clrscr
    
    ; Initialize random seed
    call Randomize
    
    ; Reset player position
    mov playerX, PLAYER_START_X
    mov playerY, PLAYER_START_Y
    
    ; Clear enemies array
    mov ecx, MAX_ENEMIES * 4
    mov esi, OFFSET enemies
    ClearEnemies:
        mov DWORD PTR [esi], 0
        add esi, 4
        loop ClearEnemies
    
    ; Clear bullets array
    mov ecx, MAX_BULLETS * 4
    mov esi, OFFSET bullets
    ClearBullets:
        mov DWORD PTR [esi], 0
        add esi, 4
        loop ClearBullets
    
    ; Reset score
    mov score, 0
    
    ret
InitializeGame ENDP

; Clear the screen buffer
ClearScreenBuffer PROC
    mov ecx, SCREEN_WIDTH * SCREEN_HEIGHT
    mov esi, OFFSET screenBuffer
    ClearLoop:
        mov BYTE PTR [esi], ' '
        inc esi
        loop ClearLoop
    ret
ClearScreenBuffer ENDP

; Handle keyboard input
HandleInput PROC
    ; Check if key is available (non-blocking)
    call KeyboardHit
    cmp eax, 0
    je NoInput
    
    ; Read the key
    call ReadChar
    mov inputChar, al
    
    mov al, inputChar
    
    ; Convert to uppercase
    cmp al, 'a'
    jl CheckOtherKeys
    cmp al, 'z'
    jg CheckOtherKeys
    sub al, 32  ; Convert to uppercase
    
    CheckOtherKeys:
    ; Check for movement keys
    cmp al, 'A'
    je MoveLeft
    cmp al, 'D'
    je MoveRight
    cmp al, ' '
    je Shoot
    cmp al, 'Q'
    je QuitGame
    jmp NoInput
    
    MoveLeft:
        cmp playerX, 1
        jle NoInput
        dec playerX
        jmp NoInput
    
    MoveRight:
        mov eax, playerX
        cmp eax, SCREEN_WIDTH - 2
        jge NoInput
        inc playerX
        jmp NoInput
    
    Shoot:
        call FireBullet
        jmp NoInput
    
    QuitGame:
        mov gameRunning, 0
        jmp NoInput
    
    NoInput:
    ret
HandleInput ENDP

; Update player (currently just placeholder)
UpdatePlayer PROC
    ; Player updates handled in input
    ret
UpdatePlayer ENDP

; Fire a bullet from player position
FireBullet PROC
    ; Find empty bullet slot
    mov ecx, MAX_BULLETS
    mov esi, OFFSET bullets
    add esi, 8  ; Skip to 'active' field of first bullet
    
    FindEmptyBullet:
        cmp DWORD PTR [esi], 0  ; Check if bullet is inactive
        je FoundEmptyBullet
        add esi, 16  ; Move to next bullet (4 fields * 4 bytes)
        loop FindEmptyBullet
        jmp NoBulletSlot
    
    FoundEmptyBullet:
        sub esi, 8  ; Go back to x field
        mov eax, playerX
        mov [esi], eax          ; Set bullet x
        mov eax, playerY
        dec eax
        mov [esi+4], eax        ; Set bullet y
        mov DWORD PTR [esi+8], 1  ; Set active
        mov DWORD PTR [esi+12], -1 ; Set direction (upward)
    
    NoBulletSlot:
    ret
FireBullet ENDP

; Update all bullets
UpdateBullets PROC
    mov ecx, MAX_BULLETS
    mov esi, OFFSET bullets
    
    UpdateBulletLoop:
        push ecx
        push esi
        
        ; Check if bullet is active
        cmp DWORD PTR [esi+8], 0
        je NextBullet
        
        ; Move bullet
        mov eax, [esi+12]  ; Get direction
        add [esi+4], eax   ; Update y position
        
        ; Check if bullet is off screen
        cmp DWORD PTR [esi+4], 0
        jl DeactivateBullet
        mov eax, [esi+4]
        cmp eax, SCREEN_HEIGHT
        jge DeactivateBullet
        jmp NextBullet
        
        DeactivateBullet:
            mov DWORD PTR [esi+8], 0  ; Deactivate bullet
        
        NextBullet:
        pop esi
        pop ecx
        add esi, 16  ; Move to next bullet
        loop UpdateBulletLoop
    
    ret
UpdateBullets ENDP

; Spawn enemies occasionally
SpawnEnemies PROC
    ; Generate random number to decide if we should spawn
    mov eax, 100
    call RandomRange
    cmp eax, ENEMY_SPAWN_RATE  ; Configurable spawn rate
    jg NoSpawn
    
    ; Find empty enemy slot
    mov ecx, MAX_ENEMIES
    mov esi, OFFSET enemies
    add esi, 8  ; Skip to 'active' field
    
    FindEmptyEnemy:
        cmp DWORD PTR [esi], 0
        je FoundEmptyEnemy
        add esi, 16  ; Move to next enemy
        loop FindEmptyEnemy
        jmp NoSpawn
    
    FoundEmptyEnemy:
        sub esi, 8  ; Go back to x field
        ; Random x position
        mov eax, SCREEN_WIDTH - 2
        call RandomRange
        inc eax
        mov [esi], eax          ; Set enemy x
        mov DWORD PTR [esi+4], 1   ; Set enemy y (top of screen)
        mov DWORD PTR [esi+8], 1   ; Set active
        mov DWORD PTR [esi+12], 0  ; Set move counter
    
    NoSpawn:
    ret
SpawnEnemies ENDP

; Update all enemies
UpdateEnemies PROC
    mov ecx, MAX_ENEMIES
    mov esi, OFFSET enemies
    
    UpdateEnemyLoop:
        push ecx
        push esi
        
        ; Check if enemy is active
        cmp DWORD PTR [esi+8], 0
        je NextEnemy
        
        ; Increment move counter
        inc DWORD PTR [esi+12]
        
        ; Move enemy down every few frames
        mov eax, [esi+12]
        and eax, 3  ; Move every 4 frames
        cmp eax, 0
        jne NextEnemy
        
        inc DWORD PTR [esi+4]  ; Move down
        
        ; Check if enemy is off screen
        mov eax, [esi+4]
        cmp eax, SCREEN_HEIGHT
        jl NextEnemy
        
        ; Deactivate enemy
        mov DWORD PTR [esi+8], 0
        
        NextEnemy:
        pop esi
        pop ecx
        add esi, 16  ; Move to next enemy
        loop UpdateEnemyLoop
    
    ret
UpdateEnemies ENDP

; Check collisions between bullets and enemies, and enemies and player
CheckCollisions PROC
    ; First, check bullet-enemy collisions
    ; Check each bullet against each enemy
    mov ecx, MAX_BULLETS
    mov esi, OFFSET bullets
    
    BulletLoop:
        push ecx
        push esi
        
        ; Check if bullet is active
        cmp DWORD PTR [esi+8], 0
        je NextBulletCollision
        
        ; Check this bullet against all enemies
        mov edx, MAX_ENEMIES
        mov edi, OFFSET enemies
        
        EnemyLoop:
            push edx
            push edi
            
            ; Check if enemy is active
            cmp DWORD PTR [edi+8], 0
            je NextEnemyCollision
            
            ; Check collision (same x and y)
            mov eax, [esi]    ; Bullet x
            mov ebx, [edi]    ; Enemy x
            cmp eax, ebx
            jne NextEnemyCollision
            
            mov eax, [esi+4]  ; Bullet y
            mov ebx, [edi+4]  ; Enemy y
            cmp eax, ebx
            jne NextEnemyCollision
            
            ; Collision detected!
            mov DWORD PTR [esi+8], 0  ; Deactivate bullet
            mov DWORD PTR [edi+8], 0  ; Deactivate enemy
            add score, 10             ; Increase score
            
            NextEnemyCollision:
            pop edi
            pop edx
            add edi, 16
            dec edx
            jnz EnemyLoop
        
        NextBulletCollision:
        pop esi
        pop ecx
        add esi, 16
        loop BulletLoop
    
    ; Check enemy-player collisions
    mov ecx, MAX_ENEMIES
    mov esi, OFFSET enemies
    
    PlayerCollisionLoop:
        push ecx
        push esi
        
        ; Check if enemy is active
        cmp DWORD PTR [esi+8], 0
        je NextPlayerCollision
        
        ; Check collision with player
        mov eax, [esi]      ; Enemy x
        mov ebx, playerX    ; Player x
        cmp eax, ebx
        jne CheckPlayerY
        
        mov eax, [esi+4]    ; Enemy y
        mov ebx, playerY    ; Player y
        cmp eax, ebx
        jne CheckPlayerY
        
        ; Direct collision - game over
        mov gameRunning, 0
        jmp NextPlayerCollision
        
        CheckPlayerY:
        ; Check if enemy reached bottom (past player)
        mov eax, [esi+4]
        cmp eax, SCREEN_HEIGHT
        jl NextPlayerCollision
        ; Enemy got past player - could deduct points or end game
        ; For now, just deactivate the enemy (already handled in UpdateEnemies)
        
        NextPlayerCollision:
        pop esi
        pop ecx
        add esi, 16
        loop PlayerCollisionLoop
    
    ret
CheckCollisions ENDP

; Render all game objects to screen buffer
RenderGame PROC
    ; Render player
    mov eax, playerY
    mov ebx, SCREEN_WIDTH
    mul ebx
    add eax, playerX
    mov esi, OFFSET screenBuffer
    add esi, eax
    mov al, playerChar
    mov [esi], al
    
    ; Render enemies
    mov ecx, MAX_ENEMIES
    mov esi, OFFSET enemies
    
    RenderEnemyLoop:
        push ecx
        push esi
        
        cmp DWORD PTR [esi+8], 0  ; Check if active
        je NextEnemyRender
        
        ; Calculate screen position
        mov eax, [esi+4]  ; y
        mov ebx, SCREEN_WIDTH
        mul ebx
        add eax, [esi]    ; x
        
        ; Bounds check
        cmp eax, SCREEN_WIDTH * SCREEN_HEIGHT
        jge NextEnemyRender
        
        mov edi, OFFSET screenBuffer
        add edi, eax
        mov al, enemyChar
        mov [edi], al
        
        NextEnemyRender:
        pop esi
        pop ecx
        add esi, 16
        loop RenderEnemyLoop
    
    ; Render bullets
    mov ecx, MAX_BULLETS
    mov esi, OFFSET bullets
    
    RenderBulletLoop:
        push ecx
        push esi
        
        cmp DWORD PTR [esi+8], 0  ; Check if active
        je NextBulletRender
        
        ; Calculate screen position
        mov eax, [esi+4]  ; y
        cmp eax, 0
        jl NextBulletRender
        cmp eax, SCREEN_HEIGHT
        jge NextBulletRender
        
        mov ebx, SCREEN_WIDTH
        mul ebx
        add eax, [esi]    ; x
        
        ; Bounds check
        cmp eax, SCREEN_WIDTH * SCREEN_HEIGHT
        jge NextBulletRender
        
        mov edi, OFFSET screenBuffer
        add edi, eax
        mov al, bulletChar
        mov [edi], al
        
        NextBulletRender:
        pop esi
        pop ecx
        add esi, 16
        loop RenderBulletLoop
    
    ret
RenderGame ENDP

; Display the screen buffer
DisplayScreen PROC
    ; Clear screen and move cursor to top
    call Clrscr
    mov dl, 0
    mov dh, 0
    call Gotoxy
    
    ; Display title and score on one line
    mov edx, OFFSET titleMsg
    call WriteString
    mov al, ' '
    call WriteChar
    mov al, ' '
    call WriteChar
    mov edx, OFFSET scoreMsg
    call WriteString
    mov eax, score
    call WriteDec
    call Crlf
    call Crlf
    
    ; Display game area
    mov esi, OFFSET screenBuffer
    mov ecx, SCREEN_HEIGHT
    
    DisplayRowLoop:
        push ecx
        push esi
        
        ; Display one row
        mov ecx, SCREEN_WIDTH
        DisplayColLoop:
            mov al, [esi]
            call WriteChar
            inc esi
            loop DisplayColLoop
        
        call Crlf
        pop esi
        pop ecx
        loop DisplayRowLoop
    
    call Crlf
    ; Display instructions
    mov edx, OFFSET instructionsMsg
    call WriteString
    
    ret
DisplayScreen ENDP

; Display game over screen
DisplayGameOver PROC
    call Clrscr
    
    mov edx, OFFSET gameOverMsg
    call WriteString
    mov eax, score
    call WriteDec
    call Crlf
    
    mov edx, OFFSET instructionsMsg
    call WriteString
    call Crlf
    
    ; Wait for any key
    call ReadChar
    
    ret
DisplayGameOver ENDP

END main