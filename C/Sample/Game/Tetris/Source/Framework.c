#include "Framework.h"



static GameStatus gameStatus;

static EventSystem* eventSystem;


static Void OnReset()
{
	FMemory.Clear( &gameStatus, sizeof(GameStatus) );
	{
		gameStatus.Next				= GetRandomElement();
		gameStatus.NextOrientation	= GetRandomElementOrientation();

		gameStatus.timeScale	= 1;
		gameStatus.isPause		= False;
		gameStatus.isGameOver	= False;
	}
	
	CreateGameElementAndResetPosition( &gameStatus );
}

Void GameInit(Void* userParam)
{
	eventSystem = CAST(EventSystem*, userParam);

	OnReset();
}

Void GameUpdate(RealType deltaTime)
{
	if ( gameStatus.isGameOver )
	{
		if ( FEventSystem.KeyClicked( eventSystem, Key_R ) )
			OnReset();

		return;
	}

	if ( FEventSystem.KeyClicked( eventSystem, Key_ESC ) )
		gameStatus.isPause = !gameStatus.isPause;

	if ( gameStatus.isPause )
		return;

	if ( FEventSystem.KeyClicked( eventSystem, Key_LeftArrow ) )
		TryMove( &gameStatus, -1, 0 );

	if ( FEventSystem.KeyClicked( eventSystem, Key_RightArrow ) )
		TryMove( &gameStatus, 1, 0 );

	if ( FEventSystem.KeyClicked( eventSystem, Key_UpArrow ) )
		TryRotate( &gameStatus );

	if ( FEventSystem.KeyPressed( eventSystem, Key_DownArrow ) )
		gameStatus.timeScale = 10;
	else
		gameStatus.timeScale = 1;


	static RealType elapsed	= 0.0;
	elapsed					+= deltaTime * gameStatus.timeScale;

	if ( elapsed > AFrameDuration )
	{
		elapsed = 0.0;

		TryUpdateMask( &gameStatus );

		if ( gameStatus.isGameOver )
			return;

		TryMove( &gameStatus, 0, -1 );
	}
}

Void GameRender(Serface* serface)
{
	// draw element
	{
		const Vector2* points = GetElementPoints( gameStatus.Current, gameStatus.CurrentOrientation );

		for ( SizeType index = 0; index < ElementGrids; index++ )
		{
			Int32 x = points[index].x + gameStatus.positionX;
			Int32 y = points[index].y + gameStatus.positionY;

			DrawGrid( serface, x, y, GridSize, 2, &FColor.Red );
		}
	}

	// draw mask
	for ( SizeType y = 0; y < GridsInY; y++ )
		for ( SizeType x = 0; x < GridsInX; x++ )
			if ( gameStatus.Mask[ GridsInX * y + x ] )
				DrawGrid( serface, x, y, GridSize, 1, &FColor.Yellow );

	// base frame
	{
		// left side's border
		for ( SizeType y = 0; y < FrameHeight; y++ )
		{
			RealType t	= CAST( RealType, FrameHeight - 1 - y ) / ( FrameHeight - 1 );
			Color color	= FColor.Multiply( &FColor.White, t );

			FSerface.SetValue( serface, FrameWidth - 1, y, &color );
		}

		// description
		{
			SizeType xOffset = FrameWidth * 1.1;
			SizeType yOffset = FrameHeight * 0.8;

			// score
			{
				static const Char8* text = "Score: ";

				Vector2 position;
				position.x = xOffset;
				position.y = yOffset;

				FText.Render( serface, text, &position, &FColor.White );


				static Char8 buffer[16];
				//_itoa_s(gameStatus.score, buffer, sizeof(buffer), 10);
				FStringConversion.TryConvertInt32( buffer, sizeof(buffer), gameStatus.score );

				position.x += FAsciiGlyphInfo.Width * FUnicode.GetUTF8ElementCount(text, 0x00);

				FText.Render( serface, buffer, &position, &FColor.Yellow );
			}

			// next
			{
				Vector2 position;
				position.x = xOffset;
				position.y = yOffset - FAsciiGlyphInfo.Height * 1.5;

				FText.Render( serface, "Next: ", &position, &FColor.White );
			}

			// preview element
			{
				const Vector2* points = GetElementPoints( gameStatus.Next, gameStatus.NextOrientation );

				const SizeType size	= GridSize * 0.75F;
				const SizeType gap	= 1;

				for ( SizeType index = 0; index < ElementGrids; index++ )
				{
					Int32 x = xOffset + points[index].x * size + size * 2;
					Int32 y = yOffset + points[index].y * size - size * 4;

					Int32 xEnd = x + size - gap;
					Int32 yEnd = y + size - gap;
					for ( SizeType xBegin = x; xBegin < xEnd; xBegin++ )
						for ( SizeType yBegin = y; yBegin < yEnd; yBegin++ )
							FSerface.SetValue( serface, xBegin, yBegin, &FColor.White );
				}
			}
		}
	}

	// game pause
	if( gameStatus.isPause )
	{
		// apply a simple post effect
		for ( SizeType index = 0; index < serface->elementCount; index++ )
		{
			Color* color = CAST( Color*, serface->buffer ) + index;

			*color = FColor.Darkened(color, 0.75);
		}

		// description
		{
			const Char8* text = "PAUSE";

			Vector2 position;
			position.x = serface->width / 2 - FAsciiGlyphInfo.Width * FUnicode.GetUTF8ElementCount(text, 0x00) / 2;
			position.y = serface->height / 2 - FAsciiGlyphInfo.Height / 2;

			FText.Render( serface, text, &position, &FColor.White );
		}
	}

	// game over
	if( gameStatus.isGameOver )
	{
		// apply a simple post effect
		for ( SizeType index = 0; index < serface->elementCount; index++ )
		{
			Color* color		= CAST( Color*, serface->buffer ) + index;
			RealType luminance	= FColor.Luminance(color);

			for ( SizeType channel = 0; channel < 3; channel++ )
				color->components[channel] = luminance;
		}

		// description
		{
			static const Char8* gameover	= "Game Over";
			static const Char8* tocontinue	= "Press Key [R] To Continue";

			Vector2 position;
			position.x = serface->width / 2 - FAsciiGlyphInfo.Width * FUnicode.GetUTF8ElementCount(gameover, 0x00) / 2;
			position.y = serface->height / 2 + FAsciiGlyphInfo.Height;

			FText.Render(serface, gameover, &position, &FColor.White);

			position.x = serface->width / 2 - FAsciiGlyphInfo.Width * FUnicode.GetUTF8ElementCount(tocontinue, 0x00) / 2;
			position.y -= FAsciiGlyphInfo.Height;

			FText.Render( serface, tocontinue, &position, &FColor.Red );
		}
	}
}
