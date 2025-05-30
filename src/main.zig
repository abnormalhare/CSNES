const SDL = @import("sdl");
const std = @import("std");
const alloc = @import("global").alloc;

const NES = @import("nes.zig").NES;
const def = @import("defines.zig");

var window: ?*SDL.SDL_Window = null;
var renderer: ?*SDL.SDL_Renderer = null;
var texture: ?*SDL.SDL_Texture = null;
var surface: ?*SDL.SDL_Surface = null;

// const VIDEO_WIDTH: i32 = 256;
// const VIDEO_HEIGHT: i32 = 240;

fn initSDL(title: []const u8, wWidth: i32, wHeight: i32, tWidth: i32, tHeight: i32) void {
    if (SDL.SDL_Init(SDL.SDL_INIT_VIDEO) < 0) {
        std.debug.print("ERROR: SDL failed to init: {s}\n", .{SDL.SDL_GetError()});
        return;
    }

    window = SDL.SDL_CreateWindow(@ptrCast(title), SDL.SDL_WINDOWPOS_UNDEFINED, SDL.SDL_WINDOWPOS_UNDEFINED, wWidth, wHeight, SDL.SDL_WINDOW_SHOWN);
    if (window == null) {
        std.debug.print("ERROR: Window failed to init: {s}", .{SDL.SDL_GetError()});
        return;
    }

    renderer = SDL.SDL_CreateRenderer(window, -1, SDL.SDL_RENDERER_ACCELERATED);
    if (renderer == null) {
        std.debug.print("ERROR: Renderer failed to init: {s}", .{SDL.SDL_GetError()});
        return;
    }

    texture = SDL.SDL_CreateTexture(renderer, SDL.SDL_PIXELFORMAT_RGBA32, SDL.SDL_TEXTUREACCESS_STREAMING, tWidth, tHeight);
    if (texture == null) {
        std.debug.print("ERROR: Texture failed to init: {s}", .{SDL.SDL_GetError()});
        return;
    }
}

fn exitSDL() void {
    SDL.SDL_DestroyWindow(window);
    SDL.SDL_DestroyRenderer(renderer);
    SDL.SDL_DestroyTexture(texture);
    SDL.SDL_Quit();
}

fn updateSDL(buffer: ?*const anyopaque, pitch: i32) void {
    _ = SDL.SDL_UpdateTexture(texture, null, buffer, pitch);
    _ = SDL.SDL_RenderClear(renderer);
    _ = SDL.SDL_RenderCopy(renderer, texture, null, null);
    SDL.SDL_RenderPresent(renderer);
}

fn convertToRGBA(ppuScreen: [def.SCREEN_HEIGHT * def.SCREEN_WIDTH]def.Dot) !*u8 {
    // var ppuScanline: [256]def.Dot = [_]def.Dot ** 256;
    // var scanline: [256]u8 = [_]u8{0} ** 256;
    const screen: []u8 = try alloc.alloc(u8, def.SCREEN_HEIGHT * def.SCREEN_WIDTH);

    // for (0..240) |i| {
    //     ppuScanline = ppuScreen[(i * 341)..(i * 341 + 256)];

    // }

    _ = ppuScreen;

    return @ptrCast(screen);
}

pub fn main() !void {
    const videoPitch: i32 = @sizeOf(u8) * def.SCREEN_WIDTH;
    var args = try std.process.argsWithAllocator(alloc);
    defer args.deinit();

    _ = args.next(); // exe

    var nes: *NES = undefined;
    if (args.next()) |filename| {
        if (args.next()) |scaleStr| {
            const scale: i32 = try std.fmt.parseInt(i32, scaleStr, 10);

            initSDL(filename, def.SCREEN_WIDTH * scale, def.SCREEN_HEIGHT * scale, def.SCREEN_WIDTH, def.SCREEN_HEIGHT);
            
            nes = (try NES.init(filename)).?;
        } else {
            std.debug.print("Format: NES_Zig.exe <ROM> <scale>", .{});
            return;
        }
    } else {
        std.debug.print("Format: NES_Zig.exe <ROM> <scale>", .{});
        return;
    }
    var lastTime: i128 = std.time.nanoTimestamp();

    while (nes.jam == 0) {
        const currTime: i128 = std.time.nanoTimestamp();
        
        if (currTime - lastTime >= 560) {
            nes.run();
            lastTime = currTime;

            const screen: *const u8 = try convertToRGBA(nes.ppu.screen);
            updateSDL(screen, videoPitch);
        }
    }
    std.debug.print("ERROR: NES jammed: 0x{X}", .{nes.jam});
}
