/* ****************************************************************************
 * Written for the Light-Wizzard, UnrealWordWrap is Unreal Engine Word Wrap 
 * written in C++ for Blueprints, and is Free Unlicensed Open Source.
 * https://github.com/Light-Wizzard/UnrealWordWrap
*/
#include "BFL_UnrealWordWrap.h"
// ****************************************************************************
/// <summary>
/// Load Text File relative to ProjectContentDir
/// </summary>
/// <param name="InputFileName">Relative to ProjectContentDir Input File Name</param>
/// <param name="TextOut">Text Out</param>
/// <returns>true if success, false if fail</returns>
bool UBFL_UnrealWordWrap::LoadTxtFile(FString InputFileName, FString& TextOut)
{
    TextOut.Empty();
    if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*(FPaths::ProjectContentDir() + InputFileName)))
    {
        return FFileHelper::LoadFileToString(TextOut, *(FPaths::ProjectContentDir() + InputFileName));
    }
    // You should not get here: Debug
    FString ProjectDirectory = FPaths::ProjectContentDir();
    UE_LOG(LogTemp, Error, TEXT("LoadTxtFile FilePaths: ProjectContentDir: %s"), *ProjectDirectory)
    return false;
}
// ****************************************************************************
/// <summary>
/// Save Text File relative to ProjectContentDir
/// </summary>
/// <param name="OutputFileName">Relative to ProjectContentDir Output File Name</param>
/// <param name="SaveText">Save Text to output file</param>
/// <returns>true if success, false if fail</returns>
bool UBFL_UnrealWordWrap::SaveTxtFile(FString OutputFileName, FString SaveText)
{
    return FFileHelper::SaveStringToFile(SaveText, *(FPaths::ProjectContentDir() + OutputFileName));
}
// ****************************************************************************
/// <summary>
/// Is File Exist relative to ProjectContentDir 
/// </summary>
/// <param name="FileName">File Name</param>
/// <param name="absolutePath">Absolute Path</param>
/// <returns>true if success, false if fail</returns>
bool UBFL_UnrealWordWrap::IsFile(FString FileName, FString& absolutePath)
{
    FString ProjectDirectory = FPaths::ProjectDir();
    // Prepend ProjectContentDir
    if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*(FPaths::ProjectContentDir() + FileName)))
    {
        absolutePath = FPlatformFileManager::Get().GetPlatformFile().ConvertToAbsolutePathForExternalAppForRead(*(FPaths::ProjectContentDir() + FileName));
        return true;
    }
    // You should not get here: Debug it
    ProjectDirectory = FPaths::ProjectContentDir();
    UE_LOG(LogTemp, Error, TEXT("FilePaths: ProjectContentDir: %s"), *ProjectDirectory);
    return false;
}
// ****************************************************************************
/// <summary>
/// Wrap Text at given lenght inserting br tags
/// </summary>
/// <param name="InputText">Input Text</param>
/// <param name="WrapAt">Wrap At inserting br tag</param>
/// <param name="TabLen">Tab Lenght: 0 and -1 removes tabs</param>
/// <param name="TextOut">Text Out</param>
/// <returns>true if success, false if fail</returns>
bool UBFL_UnrealWordWrap::WrapTxt(FString InputText, int32 WrapAt, int32 TabLen, FString& TextOut)
{
    // Sanity Chech If text is less than the Wrap At then return the Text
    if (InputText.Len() < 1) { TextOut.Empty(); return false; }
    else if (InputText.Len() < WrapAt) { TextOut = InputText; return true; }
    // LF Line Feed
    TCHAR TheLineFeed = '\n';
    // CR Carriage Return
    TCHAR TheCarriageReturn = '\r';
    // Tab
    TCHAR TheTab = '\t';
    // Backspace
    TCHAR TheBackspace = '\b';
    // Form Feed
    TCHAR TheFormFeed = '\f';
    // Line Feed String
    FString LF;
    LF.AppendChar(TheLineFeed);
    // Set the Index
    int32 TheIndex = 0;
    int32 TheCounter, TheBackTrack, TheLastIndex = -1;
    FString TheInputText = "";
    // Set Text Out Empty to length of string
    TextOut.Empty();
    // We have to remove all but the ending Carriage Return in CRLF and deal with Tabs
    for (TheCounter = 0; TheCounter < InputText.Len(); TheCounter++)
    {
        // CRLF \r\n
        if (InputText[TheCounter] == TheCarriageReturn)
        {
            if (InputText.Len() >= TheCounter + 1) { if (InputText[TheCounter + 1] == TheLineFeed) { continue; } }
        }
        else if (InputText[TheCounter] == TheTab || InputText[TheCounter] == TheBackspace || InputText[TheCounter] == TheFormFeed)
        {
            // Remove Tabs if less than 1
            if (TabLen > 0)
            {
                for (int32 TheTabIndex = 0; TheTabIndex < TabLen; TheTabIndex++)
                {
                    TheInputText.AppendChar(' ');
                }
                continue;
            }
            else { continue; }
        }
        else
        {
            // Text Render only allows Text, Alpha Numberics, Space, and Punctions
            // This is to sanitize Word Processor imbeded characters
            // If you need more, add them here, but test them first
            if (isalnum(InputText[TheCounter]) || isspace(InputText[TheCounter]) || ispunct(InputText[TheCounter]))
            {
                TheInputText.AppendChar(InputText[TheCounter]);
            }
            else
            {
                // There a lot of characters that you do not see in documents
                UE_LOG(LogTemp, Error, TEXT("Removed this character: %s"), InputText[TheCounter]);
            }
        }
    } // end for (TheCounter
    // While we have input text
    while (TheIndex < TheInputText.Len())
    {
        // Working with the copy of the input string
        for (TheCounter = 1; TheCounter <= WrapAt; TheCounter++)
        {
            // Check to see if it is at the end
            if (TheInputText.Len() == TheIndex)
            {
                // Note I used LF \n in file, if you want to use this for LF, or CRLF,
                // then change HTML br tage to what you want, and append a LF
                if (BreakTxt(TextOut, TextOut)) { return true; }
                else { return false; }
                return (TextOut.Len() > 0);
            }
            // Check for Line Feed tag we inserted into the original string 
            // to reset the counter and add br
            if (TheInputText[TheIndex] == TheCarriageReturn)
            {
                /* ************************************************************
                *  We are converting all CRLF to LF, normally only the last 
                *  character is a line feed, the Line Feeds get turned into the 
                *  HTML tag <br> for Unreal Engine, we remove all Tabs or replace 
                *  with spaces, codes that show up as a box becasue it does not understand it,
                *  we have a filter for this below.
                */ 
                TextOut.AppendChar(TheLineFeed); // Mark Line Feed \n
                // If the next character is another Line Feed, skip it, it is part of the CRLF
                if (TheInputText.Len() < TheIndex + 1)
                {
                    if (TheInputText[TheIndex + 1] == TheLineFeed)
                    {
                        TheCounter = 1; // Reset the Counter
                        continue;
                    }
                }
                TheCounter = 1; // Reset the Counter
            }
            else if (TheInputText[TheIndex] == TheLineFeed) // We removed any CR in an CRLF
            {
                TextOut.AppendChar(TheLineFeed); // Mark Line Feed \n
                TheCounter = 1; // Reset the Counter
            }
            else
            {
                char theCharacter = static_cast<unsigned char>(TheInputText[TheIndex]);
                // Filter out stuff you do not want, 11 is code 8203 a space with no width, ungly in UE
                // Generaly Characters are above 32, which is a space, 33 is an !, and so on
                // A is 65, 126 is an ~, so you will want to see what looks ugly
                if (theCharacter != 11)
                {
                    // Remap Characters that do not work in Unreal becasue they display as a box
                    // Switch from the fancy " to the one that you can see in UE
                    if (theCharacter == 28 || theCharacter == 29) { theCharacter = '"'; }
                    // Switch from the fancy ' to the one that you can see in UE
                    if (theCharacter == 24 || theCharacter == 25) { theCharacter = 39; }
                    TextOut.AppendChar(theCharacter);
                }
            }
            TheIndex++; // Increament the Index
        } // end for (theCounter
        // We ran to the End of this Line
        // Check for space
        if (isspace(TheInputText[TheIndex]))
        {
            TextOut.AppendChar(TheLineFeed); // Mark Line Feed
            TheCounter = 1; // Reset the Counter
        }
        else
        {
            // Set the Back Text
            FString TheBackText = TheInputText.Mid(TheIndex - 20, WrapAt);
            // Look for characters we can wrap at
            TArray<FString> TheBreakItems;
            if (TheBackText.Contains(LF))   { TheBreakItems.Add(LF);         }
            if (TheBackText.Contains(" "))  { TheBreakItems.Add(TEXT(" "));  }
            if (TheBackText.Contains("."))  { TheBreakItems.Add(TEXT("."));  }
            if (TheBackText.Contains(","))  { TheBreakItems.Add(TEXT(","));  }
            if (TheBackText.Contains(";"))  { TheBreakItems.Add(TEXT(";"));  }
            if (TheBackText.Contains("?"))  { TheBreakItems.Add(TEXT("?"));  }
            if (TheBackText.Contains("!"))  { TheBreakItems.Add(TEXT("!"));  }
            if (TheBackText.Contains(":"))  { TheBreakItems.Add(TEXT(":"));  }
            if (TheBackText.Contains("\\")) { TheBreakItems.Add(TEXT("\\")); }
            if (TheBackText.Contains("/"))  { TheBreakItems.Add(TEXT("/"));  }
            if (TheBackText.Contains("|"))  { TheBreakItems.Add(TEXT("|"));  }
            if (TheBackText.Contains("-"))  { TheBreakItems.Add(TEXT("-"));  }
            if (TheBackText.Contains("_"))  { TheBreakItems.Add(TEXT("_"));  }
            if (TheBackText.Contains("&"))  { TheBreakItems.Add(TEXT("&"));  }
            if (TheBackText.Contains("*"))  { TheBreakItems.Add(TEXT("*"));  }
            if (TheBackText.Contains("="))  { TheBreakItems.Add(TEXT("="));  }
            if (TheBackText.Contains("+"))  { TheBreakItems.Add(TEXT("+"));  }
            if (TheBackText.Contains("<"))  { TheBreakItems.Add(TEXT("<"));  }
            if (TheBackText.Contains(">"))  { TheBreakItems.Add(TEXT(">"));  }
            if (TheBackText.Contains("["))  { TheBreakItems.Add(TEXT("["));  }
            if (TheBackText.Contains("]"))  { TheBreakItems.Add(TEXT("]"));  }
            if (TheBackText.Contains("{"))  { TheBreakItems.Add(TEXT("{"));  }
            if (TheBackText.Contains("}"))  { TheBreakItems.Add(TEXT("}"));  }
            if (TheBackText.Contains("("))  { TheBreakItems.Add(TEXT("("));  }
            if (TheBackText.Contains(")"))  { TheBreakItems.Add(TEXT(")"));  }
            if (TheBackText.Contains("@"))  { TheBreakItems.Add(TEXT("@"));  }
            if (TheBackText.Contains("#"))  { TheBreakItems.Add(TEXT("#"));  }
            if (TheBackText.Contains("$"))  { TheBreakItems.Add(TEXT("$"));  }
            if (TheBackText.Contains("%"))  { TheBreakItems.Add(TEXT("%"));  }
            if (TheBackText.Contains("^"))  { TheBreakItems.Add(TEXT("^"));  }
            if (TheBackText.Contains("~"))  { TheBreakItems.Add(TEXT("~"));  }
            // This is last, not nice to break on don't, but ok for 'LongLine'
            if (TheBackText.Contains("`"))  { TheBreakItems.Add(TEXT("`"));  }
            // Check for nearest space or other character in the list
            int32 TheBackline = 0;
            // I did a lot of casting to make TheChar an FString
            FString TheChar;
            bool IsBraakable;
            for (TheBackTrack = TheIndex; TheBackTrack > 0; TheBackTrack--)
            {
                IsBraakable = false;
                TheChar.Empty();
                TheChar.AppendChar(TheInputText[TheBackTrack]);
                // Compare Breakable character to input
                for (int32 i = 0; i < TheBreakItems.Num(); i++)
                {
                    if (i > TheBreakItems.Num() - 1) { break; }
                    if (TheChar.Compare(TheBreakItems[i]) == 0)
                    {
                        IsBraakable = true;
                        break;
                    }
                } // end for i
                // Is Breakable
                if (IsBraakable)
                {
                    // Check to see if the Backline is larget than WrapAt or we loop
                    if (TheBackline <= WrapAt)
                    {
                        if (TheLastIndex == -1 || TheLastIndex != TheBackTrack)
                        {
                            TheLastIndex = TheBackTrack;
                        }
                        else
                        {
                            if (TheLastIndex == TheBackTrack)
                            {
                                // Reset the for loop to prevent an endless loop
                                TheBackTrack = TheIndex;
                                TheBackline = 0;
                                continue;
                            }
                        }
                        int32 TheLen = TextOut.Len();
                        // Remove all characters going back to the space
                        TextOut.RemoveAt(TheLen + 1 - (TheIndex - TheBackTrack), TheIndex - TheBackTrack);
                        TextOut.AppendChar(TheLineFeed); // Mark Line Feed
                        TheCounter = 1; // Reset the Counter
                        TheIndex = TheBackTrack; // Reset the Index
                        break;
                    }
                    else
                    {
                        if (TextOut.Len() + TheBackText.Len() >= TheInputText.Len())
                        {  
                            TextOut.Append(TheBackText); // Mark Line Feed
                            if (BreakTxt(TextOut, TextOut)) { return true; }
                            else { return false; }
                        }
                        // Reset the for loop
                        TheBackTrack = TheIndex;
                        TheBackline = 0;
                        continue;
                    } // end if (TheBackline <= WrapAt)
                } // end if (TheChar.Compare(
                TheBackline++;
            } // end for (TheBackTrack
        } // end if (isspace(TheInputText[theIndex]))
    } // end while (theIndex < TheInputText.Len())
    // We should not hit this code unless the size is too small
    if (BreakTxt(TextOut, TextOut)) { return true; }
    else { return false; }
} // end WrapTxt
// ****************************************************************************
/// <summary>
/// Break Text
/// </summary>
/// <param name="InputText">Input Text</param>
/// <param name="TheTextOut">The Text Out</param>
/// <returns>true if success, false if fail</returns>
bool UBFL_UnrealWordWrap::BreakTxt(FString InputText, FString& TheTextOut)
{
    // LF Line Feed
    TCHAR TheLineFeed = '\n';
    // Line Feed String
    FString LF;
    LF.AppendChar(TheLineFeed);
    FString TheChar;
    TheTextOut.Empty(); 
    for (int32 TheCounter = 0; TheCounter < InputText.Len(); TheCounter++)
    {
        TheChar.Empty();
        TheChar.AppendChar(InputText[TheCounter]);
        if (TheChar.Compare(LF) == 0) { TheTextOut.Append("<br>"); }
        else { TheTextOut.Append(TheChar); }
    }
    return (TheTextOut.Len() > 0);
}
// ***************************** End of File **********************************
