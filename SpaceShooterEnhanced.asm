TITLE Enhanced Space Shooter Game - x86 MASM with Irvine Library
; Enhanced version with borders, better graphics, and additional features
; Author: Generated for ammarasadPersonal
; Date: 2024

INCLUDE Irvine32.inc

.data
    ; Game constants
    SCREEN_WIDTH = 78     ; Leave space for borders
    SCREEN_HEIGHT = 20    ; Leave space for UI and borders
    MAX_ENEMIES = 15
    MAX_BULLETS = 25
    MAX_STARS = 50        ; Background stars
    PLAYER_START_X = 39
    PLAYER_START_Y = 18
    ENEMY_SPAWN_RATE = 6
    
    ; Game variables
    playerX DWORD PLAYER_START_X
    playerY DWORD PLAYER_START_Y
    playerChar BYTE '*'
    
    score DWORD 0
    lives DWORD 3
    gameRunning DWORD 1
    frameCounter DWORD 0
    
    ; Enemy array structure: [x, y, active, moveCounter]
    enemies DWORD MAX_ENEMIES * 4 DUP(0)
    enemyChar BYTE 'W'
    
    ; Bullet array structure: [x, y, active, direction]
    bullets DWORD MAX_BULLETS * 4 DUP(0)
    bulletChar BYTE '|'
    
    ; Star field for background
    stars DWORD MAX_STARS * 3 DUP(0)  ; x, y, brightness
    starChars BYTE '.', '*', '+', 0
    
    ; Screen buffer
    screenBuffer BYTE SCREEN_WIDTH * SCREEN_HEIGHT DUP(' ')
    
    ; Game messages
    titleMsg BYTE "=== ENHANCED SPACE SHOOTER ===", 0
    scoreMsg BYTE "Score: ", 0
    livesMsg BYTE " Lives: ", 0
    gameOverMsg BYTE "GAME OVER! Final Score: ", 0
    pausedMsg BYTE "PAUSED - Press P to continue", 0
    instructionsMsg1 BYTE "A/D=Move SPACE=Shoot P=Pause Q=Quit", 0
    instructionsMsg2 BYTE "Destroy enemy ships (W) to earn points!", 0
    
    ; Game state
    isPaused DWORD 0
    inputChar BYTE ?
    
.code
main PROC
    ; Initialize game
    call InitializeGame
    
    ; Main game loop
    GameLoop:
        cmp gameRunning, 0
        je GameEnd
        
        ; Handle pause
        cmp isPaused, 1
        je HandlePause
        
        ; Clear screen buffer
        call ClearScreenBuffer
        
        ; Handle input
        call HandleInput
        
        ; Update game objects
        call UpdateStars
        call UpdatePlayer
        call UpdateEnemies
        call UpdateBullets
        
        ; Check collisions
        call CheckCollisions
        
        ; Spawn new enemies occasionally
        call SpawnEnemies
        
        ; Render game
        call RenderStars
        call RenderGame
        call RenderBorders
        
        ; Display screen
        call DisplayScreen
        
        ; Increment frame counter
        inc frameCounter
        
        ; Small delay
        mov eax, 40
        call Delay
        
        jmp GameLoop
    
    HandlePause:
        ; Display pause message and wait for unpause
        call DisplayPauseScreen
        call HandleInput
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
    
    ; Initialize stars
    call InitializeStars
    
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
    
    ; Reset game variables
    mov score, 0
    mov lives, 3
    mov frameCounter, 0
    mov isPaused, 0
    
    ret
InitializeGame ENDP

; Initialize star field
InitializeStars PROC
    mov ecx, MAX_STARS
    mov esi, OFFSET stars
    
    InitStarLoop:
        push ecx
        push esi
        
        ; Random X position
        mov eax, SCREEN_WIDTH
        call RandomRange
        mov [esi], eax
        
        ; Random Y position
        mov eax, SCREEN_HEIGHT
        call RandomRange
        mov [esi+4], eax
        
        ; Random brightness (0-2)
        mov eax, 3
        call RandomRange
        mov [esi+8], eax
        
        pop esi
        pop ecx
        add esi, 12
        loop InitStarLoop
    
    ret
InitializeStars ENDP

; Update star field (scrolling background)
UpdateStars PROC
    ; Only update stars occasionally for smooth scrolling
    mov eax, frameCounter
    and eax, 3
    cmp eax, 0
    jne NoStarUpdate
    
    mov ecx, MAX_STARS
    mov esi, OFFSET stars
    
    UpdateStarLoop:
        push ecx
        push esi
        
        ; Move star down
        inc DWORD PTR [esi+4]
        
        ; If star is off bottom, reset to top with new x
        mov eax, [esi+4]
        cmp eax, SCREEN_HEIGHT
        jl NextStar
        
        mov DWORD PTR [esi+4], 0  ; Reset to top
        mov eax, SCREEN_WIDTH
        call RandomRange
        mov [esi], eax            ; New random x
        
        NextStar:
        pop esi
        pop ecx
        add esi, 12
        loop UpdateStarLoop
    
    NoStarUpdate:
    ret
UpdateStars ENDP

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
    cmp al, 'P'
    je TogglePause
    cmp al, 'Q'
    je QuitGame
    jmp NoInput
    
    MoveLeft:
        cmp isPaused, 1
        je NoInput
        cmp playerX, 1
        jle NoInput
        dec playerX
        jmp NoInput
    
    MoveRight:
        cmp isPaused, 1
        je NoInput
        mov eax, playerX
        cmp eax, SCREEN_WIDTH - 2
        jge NoInput
        inc playerX
        jmp NoInput
    
    Shoot:
        cmp isPaused, 1
        je NoInput
        call FireBullet
        jmp NoInput
    
    TogglePause:
        mov eax, isPaused
        xor eax, 1
        mov isPaused, eax
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
    cmp eax, ENEMY_SPAWN_RATE
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
        mov DWORD PTR [esi+4], 0   ; Set enemy y (top of screen)
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
        and eax, 7  ; Move every 8 frames (slower)
        cmp eax, 0
        jne NextEnemy
        
        inc DWORD PTR [esi+4]  ; Move down
        
        ; Check if enemy is off screen
        mov eax, [esi+4]
        cmp eax, SCREEN_HEIGHT
        jl NextEnemy
        
        ; Deactivate enemy and lose a life
        mov DWORD PTR [esi+8], 0
        dec lives
        
        ; Check if game over
        cmp lives, 0
        jg NextEnemy
        mov gameRunning, 0
        
        NextEnemy:
        pop esi
        pop ecx
        add esi, 16  ; Move to next enemy
        loop UpdateEnemyLoop
    
    ret
UpdateEnemies ENDP

; Check collisions
CheckCollisions PROC
    ; Check bullet-enemy collisions
    mov ecx, MAX_BULLETS
    mov esi, OFFSET bullets
    
    BulletLoop:
        push ecx
        push esi
        
        cmp DWORD PTR [esi+8], 0
        je NextBulletCollision
        
        mov edx, MAX_ENEMIES
        mov edi, OFFSET enemies
        
        EnemyLoop:
            push edx
            push edi
            
            cmp DWORD PTR [edi+8], 0
            je NextEnemyCollision
            
            mov eax, [esi]    ; Bullet x
            mov ebx, [edi]    ; Enemy x
            cmp eax, ebx
            jne NextEnemyCollision
            
            mov eax, [esi+4]  ; Bullet y
            mov ebx, [edi+4]  ; Enemy y
            cmp eax, ebx
            jne NextEnemyCollision
            
            ; Collision!
            mov DWORD PTR [esi+8], 0  ; Deactivate bullet
            mov DWORD PTR [edi+8], 0  ; Deactivate enemy
            add score, 10
            
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
        
        cmp DWORD PTR [esi+8], 0
        je NextPlayerCollision
        
        mov eax, [esi]      ; Enemy x
        mov ebx, playerX
        cmp eax, ebx
        jne NextPlayerCollision
        
        mov eax, [esi+4]    ; Enemy y
        mov ebx, playerY
        cmp eax, ebx
        jne NextPlayerCollision
        
        ; Direct hit - lose a life
        mov DWORD PTR [esi+8], 0  ; Deactivate enemy
        dec lives
        
        ; Check game over
        cmp lives, 0
        jg NextPlayerCollision
        mov gameRunning, 0
        
        NextPlayerCollision:
        pop esi
        pop ecx
        add esi, 16
        loop PlayerCollisionLoop
    
    ret
CheckCollisions ENDP

; Render stars in background
RenderStars PROC
    mov ecx, MAX_STARS
    mov esi, OFFSET stars
    
    RenderStarLoop:
        push ecx
        push esi
        
        ; Calculate screen position
        mov eax, [esi+4]  ; y
        cmp eax, 0
        jl NextStarRender
        cmp eax, SCREEN_HEIGHT
        jge NextStarRender
        
        mov ebx, SCREEN_WIDTH
        mul ebx
        add eax, [esi]    ; x
        
        ; Bounds check
        cmp eax, SCREEN_WIDTH * SCREEN_HEIGHT
        jge NextStarRender
        
        ; Get star character based on brightness
        mov ebx, [esi+8]  ; brightness
        mov edi, OFFSET starChars
        add edi, ebx
        mov bl, [edi]
        
        ; Set in buffer
        mov edi, OFFSET screenBuffer
        add edi, eax
        mov [edi], bl
        
        NextStarRender:
        pop esi
        pop ecx
        add esi, 12
        loop RenderStarLoop
    
    ret
RenderStars ENDP

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
        cmp eax, 0
        jl NextEnemyRender
        cmp eax, SCREEN_HEIGHT
        jge NextEnemyRender
        
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

; Render borders around the game area
RenderBorders PROC
    ; This is a placeholder - borders would be rendered separately
    ; in a real implementation for cleaner code organization
    ret
RenderBorders ENDP

; Display the screen buffer
DisplayScreen PROC
    ; Clear screen and move cursor to top
    call Clrscr
    
    ; Display title, score, and lives
    mov edx, OFFSET titleMsg
    call WriteString
    call Crlf
    
    mov edx, OFFSET scoreMsg
    call WriteString
    mov eax, score
    call WriteDec
    
    mov edx, OFFSET livesMsg
    call WriteString
    mov eax, lives
    call WriteDec
    call Crlf
    call Crlf
    
    ; Display top border
    mov ecx, SCREEN_WIDTH + 2
    BorderTopLoop:
        mov al, '#'
        call WriteChar
        loop BorderTopLoop
    call Crlf
    
    ; Display game area with side borders
    mov esi, OFFSET screenBuffer
    mov ecx, SCREEN_HEIGHT
    
    DisplayRowLoop:
        push ecx
        push esi
        
        ; Left border
        mov al, '#'
        call WriteChar
        
        ; Display one row
        mov ecx, SCREEN_WIDTH
        DisplayColLoop:
            mov al, [esi]
            call WriteChar
            inc esi
            loop DisplayColLoop
        
        ; Right border
        mov al, '#'
        call WriteChar
        call Crlf
        
        pop esi
        pop ecx
        loop DisplayRowLoop
    
    ; Display bottom border
    mov ecx, SCREEN_WIDTH + 2
    BorderBottomLoop:
        mov al, '#'
        call WriteChar
        loop BorderBottomLoop
    call Crlf
    call Crlf
    
    ; Display instructions
    mov edx, OFFSET instructionsMsg1
    call WriteString
    call Crlf
    mov edx, OFFSET instructionsMsg2
    call WriteString
    
    ret
DisplayScreen ENDP

; Display pause screen
DisplayPauseScreen PROC
    ; Position cursor in middle of screen
    mov dl, 25
    mov dh, 12
    call Gotoxy
    
    mov edx, OFFSET pausedMsg
    call WriteString
    
    ret
DisplayPauseScreen ENDP

; Display game over screen
DisplayGameOver PROC
    call Clrscr
    
    mov edx, OFFSET gameOverMsg
    call WriteString
    mov eax, score
    call WriteDec
    call Crlf
    call Crlf
    
    mov edx, OFFSET instructionsMsg1
    call WriteString
    call Crlf
    
    ; Wait for any key
    call ReadChar
    
    ret
DisplayGameOver ENDP

END main