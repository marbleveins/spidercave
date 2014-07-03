using System;
using System.Collections.Generic;
using System.Linq;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Audio;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.GamerServices;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using Microsoft.Xna.Framework.Media;

namespace prueba1
{
    public class Game1 : Microsoft.Xna.Framework.Game
    {
        GraphicsDeviceManager graphics;
        SpriteBatch spriteBatch;
        GraphicsDevice device;
        int screenWidth;
        int screenHeight;
        Texture2D backgroundTex;
        Player player;

        public Game1()
        {
            graphics = new GraphicsDeviceManager(this);
            Content.RootDirectory = "Content";
        }

        protected override void Initialize()
        {
            // TODO: Add your initialization logic here
            graphics.PreferredBackBufferWidth = 640;
            graphics.PreferredBackBufferHeight = 240;

            player = new Player();
            player.Pos = new Vector2(20, 20);
            player.GravityForce = .5F;
            player.Gravity = 0;
            player.JumpForce = -4;
            graphics.IsFullScreen = false;
            graphics.ApplyChanges();
            Window.Title = "Prueba1";


            base.Initialize();
        }

        protected override void LoadContent()
        {
            // Create a new SpriteBatch, which can be used to draw textures.
            spriteBatch = new SpriteBatch(GraphicsDevice);
            device = graphics.GraphicsDevice;
            player.Tex = Content.Load<Texture2D>("playerstand");
            backgroundTex = Content.Load<Texture2D>("bg");
            screenWidth = device.PresentationParameters.BackBufferWidth;
            screenHeight = device.PresentationParameters.BackBufferHeight;

        }
        protected override void UnloadContent()
        {
        }

        protected override void Update(GameTime gameTime)
        {
            // Allows the game to exit
            if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed)
                this.Exit();



            if (Keyboard.GetState().IsKeyDown(Keys.Right))
            {
                player.Pos.X++;
            }
            if (Keyboard.GetState().IsKeyDown(Keys.Left))
            {
                player.Pos.X--;
            }

            if (!traspasa(player.Pos, backgroundTex, Color.Black, 0, 1))
            {
                player.Pos.Y += (int)player.Gravity;
                player.Gravity += player.GravityForce;
            }
            else if (Keyboard.GetState().IsKeyDown(Keys.Up))
            {
                player.Gravity = player.JumpForce;
                player.Pos.Y += (int)player.Gravity;
            }

            while (traspasa(player.Pos, backgroundTex, Color.Black, 0, 0))
            {
                player.Pos.Y--;
                player.Gravity = 0;
            }



            base.Update(gameTime);
        }

        /// <summary>
        /// This is called when the game should draw itself.
        /// </summary>
        /// <param name="gameTime">Provides a snapshot of timing values.</param>
        protected override void Draw(GameTime gameTime)
        {
            GraphicsDevice.Clear(Color.CornflowerBlue);

            // TODO: Add your drawing code here
            spriteBatch.Begin();
            drawBackground();
            drawPlayer();
            spriteBatch.End();
            base.Draw(gameTime);
        }

        private void drawBackground()
        {
            Rectangle screenRectangle = new Rectangle(0, 0, screenWidth, screenHeight);
            spriteBatch.Draw(backgroundTex, screenRectangle, Color.White);
        }

        private void drawPlayer()
        {
            spriteBatch.Draw(player.Tex, player.Pos, null, Color.White, 0, new Vector2(0, player.Tex.Height), 1, SpriteEffects.None, 0);

        }


        public static bool traspasa(Vector2 Position, Texture2D Texture, Color color, int addX, int addY)
        {
            Color[] TextureDataBG = new Color[Texture.Width * Texture.Height];
            Texture.GetData(TextureDataBG);

            if (TextureDataBG[((int)Position.Y + addY) * Texture.Width + ((int)Position.X + addX)] != color)
                return false;



            return true;
        }
    }
}
