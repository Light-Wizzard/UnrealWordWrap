/*
 * UnrealWordWrap is Unreal Engine Word Wrap written in C++ for Blueprints and is Free Unlicensed Open Source.
 * Written by Jeffrey Scott Flesher
 * https://github.com/Light-Wizzard/UnrealWordWrap
*/
#include "BFL_UnrealWordWrap.h"
// ****************************************************************************
/// <summary>
/// Load Text File relative to ProjectContentDir
/// </summary>
/// <param name="inputFileName">Relative to ProjectContentDir Input File Name</param>
/// <param name="textOut">Text Out</param>
/// <returns>true if success, false if fail</returns>
bool UBFL_UnrealWordWrap::LoadTxtFile(FString inputFileName, FString& textOut)
{
    textOut.Empty();
    if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*(FPaths::ProjectContentDir() + inputFileName)))
    {
        return FFileHelper::LoadFileToString(textOut, *(FPaths::ProjectContentDir() + inputFileName));
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
/// <param name="outputFileName">Relative to ProjectContentDir Output File Name</param>
/// <param name="saveText">Save Text to output file</param>
/// <returns>true if success, false if fail</returns>
bool UBFL_UnrealWordWrap::SaveTxtFile(FString outputFileName, FString saveText)
{
    return FFileHelper::SaveStringToFile(saveText, *(FPaths::ProjectContentDir() + outputFileName));
}
// ****************************************************************************
/// <summary>
/// Is File Exist relative to ProjectContentDir 
/// </summary>
/// <param name="fileName">File Name</param>
/// <param name="absolutePath">Absolute Path</param>
/// <returns>true if success, false if fail</returns>
bool UBFL_UnrealWordWrap::IsFile(FString fileName, FString& absolutePath)
{
    FString ProjectDirectory = FPaths::ProjectDir();
    // Prepend ProjectContentDir
    if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*(FPaths::ProjectContentDir() + fileName)))
    {
        absolutePath = FPlatformFileManager::Get().GetPlatformFile().ConvertToAbsolutePathForExternalAppForRead(*(FPaths::ProjectContentDir() + fileName));
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
/// <param name="inputText">Input Text</param>
/// <param name="wrapAt">Wrap At inserting br tag</param>
/// <param name="textOut">Text Out</param>
/// <returns>true if success, false if fail</returns>
bool UBFL_UnrealWordWrap::WrapTxt(FString inputText, int32 wrapAt, FString& textOut)
{
    // Sanity Chech
    if (inputText.Len() < 1)
    {
        textOut.Empty();
        return false;
    }
    else if (inputText.Len() < wrapAt)
    {
        // If text is less than the Wrap At then return the Text
        textOut = inputText;
        return true;
    }
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
    textOut.Empty(); 
    // We have to remove all but the ending Carriage Return in CRLF and remove Tabs
    for (TheCounter = 0; TheCounter < inputText.Len(); TheCounter++)
    {
        // CRLF \r\n
        if (inputText[TheCounter] == TheCarriageReturn)
        {
            if (inputText.Len() >= TheCounter + 1) { if (inputText[TheCounter + 1] == TheLineFeed) { continue; } }
        }
        else if (inputText[TheCounter] == TheTab || inputText[TheCounter] == TheBackspace || inputText[TheCounter] == TheFormFeed)
        {
            // Remove Tabs, Backspace and Form Feeds
            continue;
        }
        else
        {
            if (isalnum(inputText[TheCounter]) || isspace(inputText[TheCounter]) || ispunct(inputText[TheCounter]))
            {
                TheInputText.AppendChar(inputText[TheCounter]);
            }
            else
            {
                // There a lot of characters that you do not see in documents
                UE_LOG(LogTemp, Error, TEXT("Removed this character: %s"), inputText[TheCounter]);
            }
        }
    } // end for (TheCounter
    // While we have input text
    while (TheIndex < TheInputText.Len())
    {
        // Working with the copy of the input string
        for (TheCounter = 1; TheCounter <= wrapAt; TheCounter++)
        {
            // Check to see if it is at the end
            if (TheInputText.Len() == TheIndex)
            {
                // Note I used LF \n in file, if you want to use this for LF, or CRLF,
                // then change HTML br tage to what you want, and append a LF
                textOut.ReplaceInline(*LF, TEXT("<br>"));
                textOut = textOut.TrimStartAndEnd();
                return (textOut.Len() > 0);
            }
            // Check for Line Feed tag we inserted into the original string to reset the counter and add br
            if (TheInputText[TheIndex] == TheCarriageReturn)
            {
                /*
                *   We are converting all CRLF to LF, normally only the last character is a line feed,
                *   the Line Feeds get turned into the HTML tag <br> for Unreal Engine,
                *   we remove all Tabs, we have no clue as to how many spaces it repersents,
                *   all a lot of codes show up as a box becasue it does not understand it,
                *   we have a filter for this below.
                */ 
                textOut.AppendChar(TheLineFeed); // Mark Line Feed \n
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
                textOut.AppendChar(TheLineFeed); // Mark Line Feed \n
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
                    textOut.AppendChar(theCharacter);
                }
            }
            TheIndex++; // Increament the Index
        } // end for (theCounter
        // We ran to the End of this Line
        // Check for space
        if (isspace(TheInputText[TheIndex]))
        {
            textOut.AppendChar(TheLineFeed); // Mark Line Feed
            TheCounter = 1; // Reset the Counter
        }
        else
        {
            // Set the Back Text
            FString TheBackText = TheInputText.Mid(TheIndex - 20, wrapAt);
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
            if (TheBackText.Contains("`"))  { TheBreakItems.Add(TEXT("`"));  } // This is last, not nice to break on don't, but ok for 'LongLine'
            // Check for nearest space or other character in the list
            int32 TheBackline = 0;
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
                }
                // Is Breakable
                if (IsBraakable)
                {
                    // Check to see if the Backline is larget than wrapAt or we loop
                    if (TheBackline <= wrapAt)
                    {
                        if (TheLastIndex == -1 || TheLastIndex != TheBackTrack)
                        {
                            TheLastIndex = TheBackTrack;
                        }
                        else
                        {
                            if (TheLastIndex == TheBackTrack)
                            {
                                // Reset the for loop
                                TheBackTrack = TheIndex;
                                TheBackline = 0;
                                continue;
                            }
                        }
                        int32 TheLen = textOut.Len();
                        // Remove all characters going back to the space
                        textOut.RemoveAt(TheLen + 1 - (TheIndex - TheBackTrack), TheIndex - TheBackTrack);
                        textOut.AppendChar(TheLineFeed); // Mark Line Feed
                        TheCounter = 1; // Reset the Counter
                        TheIndex = TheBackTrack; // Reset the Index
                        break;
                    }
                    else
                    {
                        if (textOut.Len() + TheBackText.Len() >= TheInputText.Len())
                        {  
                            textOut.Append(TheBackText); // Mark Line Feed
                            textOut.ReplaceInline(*LF, TEXT("<br>"));
                            return true;
                        }
                        // Reset the for loop
                        TheBackTrack = TheIndex;
                        TheBackline = 0;
                        continue;
                    } // end if (TheBackline <= wrapAt)
                } // end if (TheChar.Compare(
                TheBackline++;
            } // end for (TheBackTrack
        } // end if (isspace(TheInputText[theIndex]))
    } // end while (theIndex < TheInputText.Len())
    // We should not hit this code unless the size is too small
    textOut.ReplaceInline(*LF, TEXT("<br>"));
    textOut = textOut.TrimStartAndEnd();
    return (textOut.Len() > 0);
} // end WrapTxt
// ***************************** End of File **********************************
